#ifndef __PROGTEST__

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <iterator>
#include <set>
#include <list>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>
#include <deque>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <stdexcept>
#include <condition_variable>
#include <pthread.h>
#include <semaphore.h>
#include "progtest_solver.h"
#include "sample_tester.h"

using namespace std;
#endif /* __PROGTEST__ */


class ProblemPackConfig {

public:
    ProblemPackConfig(AProblemPack pack, size_t company_id, bool solved) {
        this->pack = std::move(pack);
        this->company_id = company_id;
        this->solved = solved;
    }

    AProblemPack getPack() const { return pack; }

    size_t getCompanyId() const { return company_id; }

    void setPack(const AProblemPack &packToSet) { ProblemPackConfig::pack = packToSet; }

    void setCompanyId(size_t companyId) { company_id = companyId; }

    bool isSolved() const {
        return solved;
    }

    void setSolved(bool setSolved) {
        ProblemPackConfig::solved = setSolved;
    }

private:
    AProblemPack pack;
    size_t company_id;
    bool solved;

};

using AProblemPackConfig = std::shared_ptr<ProblemPackConfig>;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------
class COptimizer {
public:
    static bool usingProgtestSolver(void) {
        return true;
    }

    static void checkAlgorithm(AProblem problem) {
        // dummy implementation if usingProgtestSolver() returns true
    }

    void producer(const ACompany &company) {
        size_t company_Id = companies_Id.at(company);//get id of the company
        while (true) {
            AProblemPack pack = company->waitForPack();
//            this_thread::sleep_for(chrono::milliseconds(1000));
            auto pack_config = make_shared<ProblemPackConfig>(pack, companies_Id.at(company), false);
            if (pack == nullptr) {


                unique_lock locker_for_company_queue(*company_Id_mutex[companies_Id[company]]);//company queue pack
                pack_Qs_Company_Id[company_Id].emplace(pack_config);//add pack to the corres company queue for order
                locker_for_company_queue.unlock();
                unique_lock locker_count_prod_end(m_count_prod_end);//for signaling that we received one nullptr
                count_prod_ended++;
                locker_count_prod_end.unlock();
                cond_For_Working_Threads.notify_all();
//                this_thread::sleep_for(chrono::milliseconds(1000));
                break;
            }
            unique_lock locker(m_for_pack_q);//global queueu pack
            q_Problem_Pack.emplace(pack_config);
            locker.unlock();
//            printf("Producer received a pack, company id: %zu\n", company_Id);

            cond_For_Working_Threads.notify_one();
            unique_lock locker_for_company_queue(*company_Id_mutex[companies_Id[company]]);//company queue pack
            pack_Qs_Company_Id[company_Id].emplace(pack_config);//add pack to the corres company queue for order
            locker_for_company_queue.unlock();


        }
//        printf("Producer accepted all packs, company id: %zu\n", company_Id);
    }

    void consumer(const ACompany &company) {
        while (true) {
//            size_t for_debug = companies_Id[company];
            unique_lock locker(*company_Id_mutex[companies_Id[company]]);//dereference the shared_ptr<mutex>
            company_Id_cond_Var[companies_Id[company]]->wait(locker, [this, company]() {
                unique_lock locker_pack_solved(m_pack_solved);
                unique_lock locker_work_dead(m_work_thread_dead_count);
//                printf("ENTERED\n");
                bool return_value = ((count_dead_work_threads - count_work_threads == 0) ||
                                     (!pack_Qs_Company_Id[companies_Id[company]].empty() &&
                                      pack_Qs_Company_Id[companies_Id[company]].front()->isSolved()));
                locker_work_dead.unlock();
                locker_pack_solved.unlock();
                return return_value;
            });
            AProblemPackConfig pack_front = pack_Qs_Company_Id[companies_Id[company]].front();
            if (pack_front->getPack() == nullptr) {
                locker.unlock();
//                printf("EXIT AFTER COND VAR\n");
                return;//if we accepted nullptr we know that there will be no more packs
            }
//            int count = 0;
            while (!pack_Qs_Company_Id[companies_Id[company]].empty()) //else check if there are more solved packs in a row
            {
//                if (count == 1)
//                    printf("Y");
//                count++;

                AProblemPackConfig pack_front1 = pack_Qs_Company_Id[companies_Id[company]].front();
                if (pack_front1->getPack() == nullptr) {
                    return;
                }
                unique_lock locker_pack_solved(m_pack_solved);
                if (pack_front1->isSolved()) {
//                    printf("SOLVING\n");
                    locker_pack_solved.unlock();
                    locker.unlock();
                    company->solvedPack(pack_front1->getPack());
                    locker.lock();
//                    printf("Consumer solved a pack, company id: %zu\n", companies_Id[company]);

//                    printf("Consumer solved a packet, id consumer is: %zu\n", companies_Id[company]);

                    pack_Qs_Company_Id[companies_Id[company]].pop();
                } else {
                    locker_pack_solved.unlock();
                    break;
                }
            }
//            printf("WTF\n");
            locker.unlock();
        }
    }

    void markPacksAsSolvedAndNotifyConsumer(const vector<AProblemPackConfig> &problem_Pack, bool include_last) {
        unique_lock locker_pack_solved(m_pack_solved);
        for (size_t i = 0; i < problem_Pack.size() - 1; i++) {
            problem_Pack[i]->setSolved(true);
            company_Id_cond_Var[problem_Pack[i]->getCompanyId()]->notify_one();//notify consumer of company owner
        }

        if (include_last) {
            problem_Pack[problem_Pack.size() - 1]->setSolved(true);
            company_Id_cond_Var[problem_Pack[problem_Pack.size() - 1]->getCompanyId()]->notify_one();//notify cons comp
        }
        locker_pack_solved.unlock();
    }

    void doWork() {
        while (true) {
//            printf("Entered while loop again\n");
            unique_lock locker(m_for_pack_q);//for accessing queue
            unique_lock locker_prod_end(m_count_prod_end);//for accessing count_prod_ended
            if (count_prod_ended == companies.size() && q_Problem_Pack.empty()) {
//                printf("WANT TO BREAK\n");
                locker_prod_end.unlock();
                unique_lock locker_packs_in_solver(m_solver);//for accessing packs_currently_in_solver
                unique_lock locker_dead_work_threads(m_work_thread_dead_count);//for accessing count_work_threads
                if (count_work_threads - count_dead_work_threads == 1 &&//if last working thread and solver not empty
                    !packs_Currently_In_Solver.empty()) {
//                    printf("I am last\n");
                    locker_dead_work_threads.unlock();
                    vector<AProblemPackConfig> temp_vec = packs_Currently_In_Solver;
                    packs_Currently_In_Solver.clear();
                    locker_packs_in_solver.unlock();
                    solver->solve();//no need to lock solver, because we are the only one left here
                    markPacksAsSolvedAndNotifyConsumer(temp_vec, true);
                    break;
                }
//                printf("I am last\n");
                locker_dead_work_threads.unlock();
                locker_packs_in_solver.unlock();//for accessing packs_currently_in_solver
                locker.unlock();
                break;
            }
            locker_prod_end.unlock();

            cond_For_Working_Threads.wait(locker, [this]() {
                unique_lock locker_prod_end_cond(m_count_prod_end);//for accessing count_prod_ended in cond_var
                bool return_value =
                        !q_Problem_Pack.empty() || (count_prod_ended == companies.size() && q_Problem_Pack.empty());
                locker_prod_end_cond.unlock();
                return return_value;
            });
//            if(q_Problem_Pack.empty())
//            {
//                locker.unlock();
//            }
            unique_lock locker_prod_end_after_cond(m_count_prod_end);//for accessing count_prod_ended
            if ((count_prod_ended == companies.size() && q_Problem_Pack.empty()) || q_Problem_Pack.empty()) {
//                if (count_prod_ended == companies.size() && q_Problem_Pack.empty())
//                   printf("Checked for empty\n");
                locker_prod_end_after_cond.unlock();
                locker.unlock();
                continue;//just get back to beginning and break the iteration if needed
            }
            locker_prod_end_after_cond.unlock();
            AProblemPackConfig received_pack = q_Problem_Pack.front();
            q_Problem_Pack.pop();
//            printf("Working thread received a packet\n");
            locker.unlock();

            unique_lock locker_solver(m_solver);
            packs_Currently_In_Solver.push_back(received_pack);
            bool leftover = false;
            for (size_t i = 0; i < received_pack->getPack()->m_Problems.size(); i++) {
                if (leftover) {
                    packs_Currently_In_Solver.push_back(received_pack);
                    leftover = false;
                }
                if (solver->hasFreeCapacity()) {
                    controlCapacit++;
                    solver->addProblem(received_pack->getPack()->m_Problems[i]);//just adding problems from the pack
                } else {
//                    unique_lock locker_packs_in_solver(m_packs_curr_solver);//for accessing packs_currently_in_solver
                    vector<AProblemPackConfig> temp_vec = packs_Currently_In_Solver;
                    packs_Currently_In_Solver.clear();
//                    locker_packs_in_solver.unlock();

//                    printf("Control capacity: %zu\n", controlCapacit);
                    AProgtestSolver temp_solver = solver;
                    solver = createProgtestSolver();
                    locker_solver.unlock();
                    temp_solver->solve();
                    locker_solver.lock();
                    markPacksAsSolvedAndNotifyConsumer(temp_vec, false);
                    leftover = true;
                    i--;
                }
            }
            //check if last problem in the pack took last free space in progtestSolver
            if (!solver->hasFreeCapacity()) {
                vector<AProblemPackConfig> temp_vec = packs_Currently_In_Solver;
                packs_Currently_In_Solver.clear();

                AProgtestSolver temp_solver = solver;
                solver = createProgtestSolver();
                locker_solver.unlock();
                temp_solver->solve();
                locker_solver.lock();
                markPacksAsSolvedAndNotifyConsumer(temp_vec, true);
            }
            locker_solver.unlock();
        }
        unique_lock locker_dead_work(m_work_thread_dead_count);
        count_dead_work_threads++;
        locker_dead_work.unlock();
        for (const auto &cond: company_Id_cond_Var) {//notifying all
            cond.second->notify_one();
        }

    }

    void start(int threadCount) {
        count_work_threads = threadCount;
        solver = createProgtestSolver();
        for (const auto &company: companies) {
            producer_Threads.emplace_back(&COptimizer::producer, this, company);
            consumer_Threads.emplace_back(&COptimizer::consumer, this, company);
        }

        for (int i = 0; i < threadCount; i++) {
            working_Threads.emplace_back(&COptimizer::doWork, this);
        }
    }

    void stop(void) {
        for (auto &thr: producer_Threads)
            thr.join();
        for (auto &thr: working_Threads)
            thr.join();
        for (auto &thr: consumer_Threads)
            thr.join();
    }

    void addCompany(ACompany company) {
        companies_Id.emplace(company, company_Id_Counter);
        companies.push_back(company);
        shared_ptr<mutex> mtx = make_shared<mutex>();//only works with smart pointers
        shared_ptr<condition_variable> cond_var = make_shared<condition_variable>();//same with cond_vars
        company_Id_cond_Var.emplace(company_Id_Counter, cond_var);//creating cond_vars for individual companies
        company_Id_mutex.emplace(company_Id_Counter, mtx);//creating mutexes for individual companies
        pack_Qs_Company_Id.emplace(company_Id_Counter, queue<AProblemPackConfig>{});//creating queues for companies
        company_Id_Counter++;

    }

private:
    size_t controlCapacit = 0;
    vector<ACompany> companies;

    vector<thread> producer_Threads;
    vector<thread> consumer_Threads;
    vector<thread> working_Threads;

    mutex m_for_pack_q;//mutex for adding packs to the queue
    mutex m_solver;//mutex for adding problems to the solver
    mutex m_count_prod_end;//mutex for counting how many producers have ended
    mutex m_packs_curr_solver;//mutex for packs_Currently_In_Solver
    mutex m_pack_solved;//mutex for packs_Solved
    mutex m_work_thread_dead_count;//mutex for packs_Solved


    queue<AProblemPackConfig> q_Problem_Pack;//deque for storing packs

    condition_variable cond_For_Working_Threads;//cond var for awakening working threads when there are packs in q
    condition_variable cond_For_Consumer;//cond var for awaking consumer when we solved packs

    AProgtestSolver solver;

    unordered_map<ACompany, size_t> companies_Id;//map companies to it`s unique ids
    unordered_map<size_t, queue<AProblemPackConfig>> pack_Qs_Company_Id;//map ids to the queue of packs accepted by producer(to save order)
    unordered_map<size_t, shared_ptr<condition_variable>> company_Id_cond_Var;//map company ids to the cond_var, so we know what company to wake up
    unordered_map<size_t, shared_ptr<mutex>> company_Id_mutex;//map company ids to the mutex, so we know what mutex pass to the corresponding cond_var

    size_t count_prod_ended = 0;
    size_t company_Id_Counter = 0;
    size_t count_work_threads = 0;
    size_t count_dead_work_threads = 0;
    vector<AProblemPackConfig> packs_Currently_In_Solver;//vector of packs currently in the solver


};

// TODO: COptimizer implementation goes here
//-------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef __PROGTEST__

int main(void) {
    COptimizer optimizer;
    ACompanyTest company = std::make_shared<CCompanyTest>();
    ACompanyTest company1 = std::make_shared<CCompanyTest>();
    ACompanyTest company2 = std::make_shared<CCompanyTest>();
    ACompanyTest company3 = std::make_shared<CCompanyTest>();
    ACompanyTest company4 = std::make_shared<CCompanyTest>();
    ACompanyTest company5 = std::make_shared<CCompanyTest>();
    ACompanyTest company6 = std::make_shared<CCompanyTest>();
    ACompanyTest company7 = std::make_shared<CCompanyTest>();
    ACompanyTest company8 = std::make_shared<CCompanyTest>();


    optimizer.addCompany(company);
    optimizer.addCompany(company1);
//    optimizer.addCompany(company2);
//    optimizer.addCompany(company3);
//    optimizer.addCompany(company4);
//    optimizer.addCompany(company5);
//    optimizer.addCompany(company6);
//    optimizer.addCompany(company7);
//    optimizer.addCompany(company8);


    optimizer.start(5);
    optimizer.stop();
    if (!company->allProcessed())
        throw std::logic_error("(some) problems were not correctly processsed");
    return 0;
}

#endif /* __PROGTEST__ */