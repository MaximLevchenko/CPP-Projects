#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h> // socket(), bind(), connect(), listen()
#include <unistd.h> // close(), read(), write()
#include <netinet/in.h> // struct sockaddr_in
#include <strings.h> // bzero()
#include <wait.h> // waitpid()
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#define BUFFER_SIZE 10240
#define TIMEOUT 1
#define TIMEOUT_RECHARGING 5

using namespace std;

string final_chars = "\\a\\b";  // End of message delimiter
#define FINE 1
#define BAD (-3)

// Global variables for managing state and client information
unordered_map<int, pair<int, int>> key_id_map;
bool on_zero_zero = false;
bool present_received = false;
bool already_hit_y_zero = false;
bool received_recharging = false;

/**
 * Converts a string to its ASCII sum, considering escape sequences.
 *
 * @param word The string to convert.
 * @return The cumulative ASCII value of the string.
 */
 size_t convertToASCII(const string &word) {
    size_t res_ASCII_value = 0;
    for (size_t i = 0; i < word.size(); i++) {
        size_t value_to_add;
        if (word[i] == '\\' && (word[i + 1] == 'a')) {
            value_to_add = int('\a');
            i++;
        } else if (word[i] == '\\' && (word[i + 1] == 'b')) {
            value_to_add = int('\b');
            i++;
        } else if (word[i] == '\\' && (word[i + 1] == '0')) {
            value_to_add = int('\0');
            i++;
        } else
            value_to_add = int(word[i]);
        res_ASCII_value += value_to_add;
    }
    return res_ASCII_value - int('\a') - int('\b'); // Subtracting last \a\b
}

/**
 * Extracts a single integer from a string.
 *
 * @param str The string to extract the integer from.
 * @return The extracted integer or a special error code.
 */
int extractIntegerWord(const string &str) {
    stringstream ss(str);
    string temp;
    int found;
    while (ss >> temp) {
        if (stringstream(temp) >> found) {
            return found;
        }
    }
    return BAD;
}

/**
 * Extracts all integers from a string and returns them in a vector.
 *
 * @param str The string to extract integers from.
 * @return A vector containing all extracted integers.
 */
vector<int> extractIntegerWords(const string &str) {
    stringstream ss(str);
    vector<int> x_y;
    string temp;
    int found;
    while (ss >> temp) {
        if (stringstream(temp) >> found) {
            x_y.emplace_back(found);
        }
    }
    return x_y;
}

/**
 * Initializes a map with predefined key-id pairs for authentication.
 */
void createKeyIdMap() {
    key_id_map.emplace(0, pair<int, int>(23019, 32037));
    key_id_map.emplace(1, pair<int, int>(32037, 29295));
    key_id_map.emplace(2, pair<int, int>(18789, 13603));
    key_id_map.emplace(3, pair<int, int>(16443, 29533));
    key_id_map.emplace(4, pair<int, int>(18189, 21952));
}

/**
 * Extracts individual messages from a received string, which may contain multiple messages.
 *
 * @param full_received_string The complete received string that may contain multiple messages.
 * @return A vector containing all extracted messages.
 */
 vector<string> getAllMessages(string &full_received_string) {
    size_t curr_pos = 0;
    vector<string> all_messages;
    while (true) {
        size_t before_pos = curr_pos;
        curr_pos = full_received_string.find(final_chars, curr_pos);
        if (curr_pos == string::npos) {
            string leftover = full_received_string.substr(before_pos, string::npos);
            if (leftover.empty()) {
                full_received_string.clear();
                break;
            }
            full_received_string = leftover;
            break;
        }
        curr_pos += 4; // Skipping \a\b
        size_t msg_len = curr_pos - before_pos;
        all_messages.emplace_back(full_received_string.substr(before_pos, msg_len));
    }
    return all_messages;
}

/**
 * Calculates a hash based on the ASCII value and the client's key ID for authentication.
 *
 * @param ASCII_number The ASCII sum of the username.
 * @param CLIENT_KEY_ID The client's key ID.
 * @param id_who Specifies whether the calculation is for the client or the server.
 * @return The calculated hash value.
 */
 size_t calculateHash(const size_t &ASCII_number, int CLIENT_KEY_ID, const string &id_who) {
    if (id_who == "server") {
        return (ASCII_number * 1000 + key_id_map.at(CLIENT_KEY_ID).first) % 65536;
    } else {
        return (ASCII_number * 1000 + key_id_map.at(CLIENT_KEY_ID).second) % 65536;
    }
}

/**
 * Sends a message to the client.
 *
 * @param c The client socket.
 * @param buffer_send The message buffer to send.
 * @return FINE if successful, BAD if an error occurred.
 */
 int sendMsg(int &c, char buffer_send[BUFFER_SIZE]) {
    cout << "> ";
    if (send(c, buffer_send, strlen(buffer_send), MSG_NOSIGNAL) < 0) {
        perror("Nemohu odeslat data:");
        close(c);
        return BAD;
    }
    return FINE;
}



/**
 * Detects if the robot's coordinates have changed and determines its facing direction.
 *
 * @param vector_curr_coords The current coordinates of the robot.
 * @param vector_before_coords The previous coordinates of the robot.
 * @param robot_facing A reference to the string indicating the robot's facing direction.
 * @param msg_type_value The map storing message type and value pairs.
 * @return True if the coordinates changed and facing direction was determined, otherwise false.
 */
 bool detectCoordsChangeAngGetRobotFacing(const vector<int> &vector_curr_coords, const vector<int> &vector_before_coords,
                                         string &robot_facing, unordered_map<string, string> &msg_type_value) {
    int x_curr = vector_curr_coords[0], y_curr = vector_curr_coords[1];
    int x_before = vector_before_coords[0], y_before = vector_before_coords[1];
    if (msg_type_value.at("CLIENT_LAST_ACTION") != "MOVE") return false;
    if (x_curr > x_before) {
        robot_facing = "RIGHT";
        return true;
    } else if (x_curr < x_before) {
        robot_facing = "LEFT";
        return true;
    } else if (y_curr > y_before) {
        robot_facing = "UP";
        return true;
    } else if (y_curr < y_before) {
        robot_facing = "DOWN";
        return true;
    } else
        return false;
}

/**
 * Returns a queue of moves to go around a wall at the top.
 *
 * @param robot_facing The current facing direction of the robot.
 * @return A queue of strings representing the moves.
 */

queue<string> getMovesToGoAroundWallAtTop(const string &robot_facing) {
    queue<string> q_moves_todo;
    vector<string> temp_vec{"LEFT", "MOVE", "RIGHT", "MOVE", "MOVE", "RIGHT", "MOVE", "LEFT"};
    for (const auto &item: temp_vec)
        q_moves_todo.emplace(item);
    return q_moves_todo;
}

/**
 * Returns a queue of moves to go around a wall at the bottom.
 *
 * @param robot_facing The current facing direction of the robot.
 * @return A queue of strings representing the moves.
 */
 queue<string> getMovesToGoAroundWallAtBottom(const string &robot_facing) {
    queue<string> q_moves_todo;
    vector<string> temp_vec{"RIGHT", "MOVE", "LEFT", "MOVE", "MOVE", "LEFT", "MOVE", "RIGHT"};
    for (const auto &item: temp_vec)
        q_moves_todo.emplace(item);
    return q_moves_todo;
}

/**
 * Rotates the robot's facing direction clockwise.
 *
 * @param robot_facing The current facing direction of the robot.
 */
 void rotateCLockWise(string &robot_facing) {
    if (robot_facing == "UP")robot_facing = "RIGHT";
    else if (robot_facing == "RIGHT")robot_facing = "DOWN";
    else if (robot_facing == "DOWN")robot_facing = "LEFT";
    else robot_facing = "UP";
}

/**
 * Rotates the robot's facing direction counter-clockwise.
 *
 * @param robot_facing The current facing direction of the robot.
 */
 void rotateAntiCLockWIse(string &robot_facing) {
    if (robot_facing == "UP")robot_facing = "LEFT";
    else if (robot_facing == "RIGHT")robot_facing = "UP";
    else if (robot_facing == "DOWN")robot_facing = "RIGHT";
    else robot_facing = "DOWN";
}

/**
 * Sends an action command to the robot based on the queued actions.
 *
 * @param c The client socket.
 * @param actions_todo A queue of actions to perform.
 * @param msg_type_value A map storing message type and value pairs.
 * @param robot_facing The current facing direction of the robot.
 * @return FINE if successful, BAD if an error occurred.
 */
 int sendActionToRobot(int &c, queue<string> &actions_todo, unordered_map<string, string> &msg_type_value,
                      string &robot_facing) {
    string action = actions_todo.front();
    actions_todo.pop();
    if (action == "MOVE") {
        msg_type_value.at("CLIENT_LAST_ACTION") = "MOVE"; // Mark last action
        if (sendMsg(c, (char *) "102 MOVE\a\b") != FINE)
            return BAD;
    } else if (action == "RIGHT") {
        msg_type_value.at("CLIENT_LAST_ACTION") = "RIGHT"; // Mark last action
        rotateCLockWise(robot_facing);
        if (sendMsg(c, (char *) "104 TURN RIGHT\a\b") != FINE)
            return BAD;
    } else if (action == "LEFT") {
        msg_type_value.at("CLIENT_LAST_ACTION") = "LEFT"; // Mark last action
        rotateAntiCLockWIse(robot_facing);
        if (sendMsg(c, (char *) "103 TURN LEFT\a\b") != FINE)
            return BAD;
    } else
        return BAD;
    return FINE;
}

/**
 * Returns a queue of moves to go around a wall based on the robot's facing direction and position.
 *
 * @param robot_facing The current facing direction of the robot.
 * @param x The robot's x-coordinate.
 * @param y The robot's y-coordinate.
 * @return A queue of strings representing the moves.
 */
 queue<string> getMovesForRobotWithWallInFront(const string &robot_facing, int &x, int &y) {
    queue<string> q_moves_todo;
    if (y > 0)
        q_moves_todo = getMovesToGoAroundWallAtTop(robot_facing);
    if (y < 0)
        q_moves_todo = getMovesToGoAroundWallAtBottom(robot_facing);
    if (y == 0)
        q_moves_todo = getMovesToGoAroundWallAtTop(robot_facing);
    return q_moves_todo;
}

/**
 * Returns a queue of moves when there are no walls in front and the robot is at y = 0.
 *
 * @param robot_facing The current facing direction of the robot.
 * @param x The robot's x-coordinate.
 * @param y The robot's y-coordinate.
 * @return A queue of strings representing the moves.
 */
 queue<string> getMovesForRobotWithNoWallsInFrontWithYEqualToZero(const string &robot_facing, int &x, int &y) {
    queue<string> q_moves_todo;
    if (x < 0) {
        if (robot_facing == "DOWN")
            q_moves_todo.emplace("LEFT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "UP")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "LEFT")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "RIGHT")
            q_moves_todo.emplace("MOVE");
    } else if (x > 0) {
        if (robot_facing == "DOWN")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "UP")
            q_moves_todo.emplace("LEFT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "LEFT")
            q_moves_todo.emplace("MOVE");
        if (robot_facing == "RIGHT")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
    } else // Reached (0,0)
        on_zero_zero = true;
    return q_moves_todo;
}

/**
 * Returns a queue of moves when there are no walls in front.
 *
 * @param robot_facing The current facing direction of the robot.
 * @param x The robot's x-coordinate.
 * @param y The robot's y-coordinate.
 * @return A queue of strings representing the moves.
 */
 queue<string> getMovesForRobotWithNoWallsInFront(const string &robot_facing, int &x, int &y) {
    queue<string> q_moves_todo;
    if (y > 0) {
        if (robot_facing == "DOWN")
            q_moves_todo.emplace("MOVE");
        if (robot_facing == "UP")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "LEFT")
            q_moves_todo.emplace("LEFT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "RIGHT")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
    } else if (y < 0) {
        if (robot_facing == "DOWN")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "UP")
            q_moves_todo.emplace("MOVE");
        if (robot_facing == "LEFT")
            q_moves_todo.emplace("RIGHT"), q_moves_todo.emplace("MOVE");
        if (robot_facing == "RIGHT")
            q_moves_todo.emplace("LEFT"), q_moves_todo.emplace("MOVE");
    } else // y == 0
        q_moves_todo = getMovesForRobotWithNoWallsInFrontWithYEqualToZero(robot_facing, x, y);
    return q_moves_todo;
}

/**
 * Determines the robot's next moves based on its current state and environment.
 *
 * @param robot_facing The current facing direction of the robot.
 * @param coords_changed Whether the coordinates have changed.
 * @param last_action The last action performed by the robot.
 * @param vec_curr_coords The current coordinates of the robot.
 * @param vec_before_coords The previous coordinates of the robot.
 * @return A queue of strings representing the next moves.
 */
 queue<string> getMovesForRobot(const string &robot_facing, bool coords_changed, const string &last_action,
                               const vector<int> &vec_curr_coords, const vector<int> &vec_before_coords) {
    int y = vec_curr_coords[1];
    int x = vec_curr_coords[0];
    queue<string> q_moves_todo;
    if (robot_facing == "UNKNOWN") {
        if (!coords_changed && last_action == "MOVE") {
            q_moves_todo.emplace("RIGHT");
            q_moves_todo.emplace("MOVE");
            return q_moves_todo;
        }
        q_moves_todo.emplace("RIGHT");
        q_moves_todo.emplace("RIGHT");
        q_moves_todo.emplace("MOVE");
        return q_moves_todo;
    }
    if (!coords_changed && last_action == "MOVE") {
        q_moves_todo = getMovesForRobotWithWallInFront(robot_facing, x, y);
        return q_moves_todo;
    }
    q_moves_todo = getMovesForRobotWithNoWallsInFront(robot_facing, x, y);
    return q_moves_todo;
}

/**
 * Sends a syntax error message to the client.
 *
 * @param c The client socket.
 */
 void sendSyntaxError(int &c) {
    perror("SYNTAX_ERROR");
    string message_to_send = "301 SYNTAX ERROR\a\b";
    sendMsg(c, message_to_send.data());
}

/**
 * Counts the number of spaces in a string.
 *
 * @param str The string to count spaces in.
 * @return The number of spaces in the string.
 */
 int countSpaces(const std::string &str) {
    int space_count = 0;
    for (char c: str)
        if (c == ' ')
            space_count++;
    return space_count;
}

/**
 * Validates the key ID received from the client.
 *
 * @param c The client socket.
 * @param CLIENT_KEY_ID The key ID received from the client.
 * @return FINE if valid, BAD if an error occurred.
 */
 int validateKeyId(int &c, const string &CLIENT_KEY_ID) {
    int extracted_value = extractIntegerWord(CLIENT_KEY_ID);
    if (extracted_value > 4 || (extracted_value < 0 && extracted_value != BAD)) {
        perror("SERVER_KEY_OUT_OF_RANGE_ERROR");
        string message_to_send = "303 KEY OUT OF RANGE\a\b";
        sendMsg(c, message_to_send.data());
        return BAD;
    } else if (extracted_value == BAD) {
        sendSyntaxError(c);
        return BAD;
    }
    return FINE;
}

/**
 * Validates the secret message received from the client.
 *
 * @param c The client socket.
 * @param CLIENT_SECRET_MESSAGE The secret message received from the client.
 * @return FINE if valid, BAD if an error occurred.
 */
 int validateSecretMessage(int &c, const string &CLIENT_SECRET_MESSAGE) {
    if (CLIENT_SECRET_MESSAGE.size() > 100) {
        sendSyntaxError(c);
        return BAD;
    }
    return FINE;
}

/**
 * Validates the username received from the client.
 *
 * @param c The client socket.
 * @param CLIENT_USERNAME The username received from the client.
 * @return FINE if valid, BAD if an error occurred.
 */
 int validateUsername(int &c, const string &CLIENT_USERNAME) {
    if (CLIENT_USERNAME.size() > 20) {
        sendSyntaxError(c);
        return BAD;
    }
    return FINE;
}

/**
 * Validates the hash confirmation received from the client.
 *
 * @param c The client socket.
 * @param CLIENT_CONFIRMATION The hash confirmation received from the client.
 * @return FINE if valid, BAD if an error occurred.
 */
 int validateClientHash(int &c, const string &CLIENT_CONFIRMATION) {
    auto is_space = [](char c) { return c == ' '; };
    auto space_pos = std::find_if(CLIENT_CONFIRMATION.begin(), CLIENT_CONFIRMATION.end(), is_space);
    if (space_pos != CLIENT_CONFIRMATION.end() || CLIENT_CONFIRMATION.size() > 9) {
        sendSyntaxError(c);
        return BAD;
    }
    return FINE;
}

/**
 * Validates the coordinates received from the client.
 *
 * @param c The client socket.
 * @param curr_str_coords The coordinates received from the client.
 * @return FINE if valid, BAD if an error occurred.
 */
 int validateCoords(int &c, const string &curr_str_coords) {
    auto is_dot = [](char c) { return c == '.'; };
    auto dot_pos = std::find_if(curr_str_coords.begin(), curr_str_coords.end(), is_dot);

    if (curr_str_coords.size() > 15 || dot_pos != curr_str_coords.end() || countSpaces(curr_str_coords) > 2) {
        sendSyntaxError(c);
        return BAD;
    }
    return FINE;
}

/**
 * Validates and processes the client's hash confirmation, then initiates the robot's movement.
 *
 * @param full_received_string The full message received from the client.
 * @param msg_cnt The current message count.
 * @param msg_type_value A map storing message type and value pairs.
 * @param c The client socket.
 * @return FINE if successful, BAD if an error occurred.
 */
 int checkClientsHashAndStartMoving(const string &full_received_string, size_t &msg_cnt,
                                   unordered_map<string, string> &msg_type_value, int &c) {
    string CLIENT_CONFIRMATION = full_received_string.substr(0, full_received_string.length());
    if (validateClientHash(c, CLIENT_CONFIRMATION) != FINE) return BAD;

    msg_type_value.emplace("CLIENT_CONFIRMATION", CLIENT_CONFIRMATION);
    size_t hash = calculateHash(convertToASCII(msg_type_value.at("CLIENT_USERNAME")),
                                extractIntegerWord(msg_type_value.at("CLIENT_KEY_ID")),
                                "client");
    if (hash == extractIntegerWord(msg_type_value.at("CLIENT_CONFIRMATION"))) {
        string message_to_send = "200 OK\a\b";
        if (sendMsg(c, message_to_send.data()) != FINE)
            return BAD;
        message_to_send = "102 MOVE\a\b";
        msg_type_value.emplace("CLIENT_LAST_ACTION", "MOVE");
        if (sendMsg(c, message_to_send.data()) != FINE)
            return BAD;
        msg_cnt++;
        return FINE;
    } else {
        string message_to_send = "300 LOGIN FAILED\a\b";
        if (sendMsg(c, message_to_send.data()) != FINE)
            return BAD;
        return BAD;
    }
}

/**
 * Handles the user authorization process by validating the username, key ID, and hash confirmation.
 *
 * @param full_received_string The full message received from the client.
 * @param msg_cnt The current message count.
 * @param msg_type_value A map storing message type and value pairs.
 * @param c The client socket.
 * @return FINE if successful, BAD if an error occurred.
 */
 int authorizeUser(const string &full_received_string, size_t &msg_cnt, unordered_map<string, string> &msg_type_value,
                  int &c) {
    if (msg_cnt == 0) {
        string CLIENT_USERNAME = full_received_string.substr(0, full_received_string.length());
        if (validateUsername(c, CLIENT_USERNAME) != FINE) return BAD;
        msg_type_value.emplace("CLIENT_USERNAME", CLIENT_USERNAME);
        string message_to_send = "107 KEY REQUEST\a\b";
        if (sendMsg(c, message_to_send.data()) != FINE) return BAD;
        msg_cnt++;
        return FINE;
    } else if (msg_cnt == 1) {
        string CLIENT_KEY_ID = full_received_string.substr(0, full_received_string.length());
        if (validateKeyId(c, CLIENT_KEY_ID) != FINE) return BAD;
        msg_type_value.emplace("CLIENT_KEY_ID", CLIENT_KEY_ID);
        size_t ASCII_number = convertToASCII(msg_type_value.at("CLIENT_USERNAME"));
        size_t hash = calculateHash(ASCII_number, extractIntegerWord(msg_type_value.at("CLIENT_KEY_ID")), "server");
        string message_to_send = to_string(hash) + "\a\b";
        if (sendMsg(c, message_to_send.data()) != FINE)
            return BAD;
        msg_cnt++;
        return FINE;
    } else if (msg_cnt == 2) {
        if (checkClientsHashAndStartMoving(full_received_string, msg_cnt, msg_type_value, c) != FINE)
            return BAD;
        return FINE;
    }
    return FINE;
}

/**
 * Debugging function to print the current action queue and robot facing direction.
 *
 * @param robot_facing The current facing direction of the robot.
 * @param actions_todo The queue of actions to be performed by the robot.
 */
 void showQ(const string &robot_facing, const queue<string> &actions_todo) {
    queue<string> temp_q = actions_todo;
    while (!temp_q.empty()) {
        cout << " " << temp_q.front();
        temp_q.pop();
    }
    cout << " robot facing: " << robot_facing << endl;
}

/**
 * Manages the robot's activity by processing received coordinates and determining the next actions.
 *
 * @param full_received_string The full message received from the client.
 * @param msg_cnt The current message count.
 * @param msg_type_value A map storing message type and value pairs.
 * @param c The client socket.
 * @param robot_facing The current facing direction of the robot.
 * @param actions_todo The queue of actions to be performed by the robot.
 * @return FINE if successful, BAD if an error occurred.
 */
 int manageRobotActivity(const string &full_received_string, size_t &msg_cnt,
                        unordered_map<string, string> &msg_type_value, int &c, string &robot_facing,
                        queue<string> &actions_todo) {
    if (validateCoords(c, full_received_string) != FINE)
        return BAD;
    vector<int> vector_curr_coords = extractIntegerWords(full_received_string);
    vector<int> vector_before_coords;
    string str_curr_coords;
    for (const auto &coord: vector_curr_coords)
        str_curr_coords += to_string(coord) + " ";
    if (!msg_type_value.count("CLIENT_COORDS")) {
        msg_type_value.emplace("CLIENT_COORDS", str_curr_coords);
        actions_todo = getMovesForRobot(robot_facing, on_zero_zero, msg_type_value.at("CLIENT_LAST_ACTION"),
                                        vector_curr_coords, vector_before_coords);
        if (sendActionToRobot(c, actions_todo, msg_type_value, robot_facing) != FINE)
            return BAD;
        return FINE;
    }
    vector_before_coords = extractIntegerWords(msg_type_value.at("CLIENT_COORDS"));
    msg_type_value.at("CLIENT_COORDS") = str_curr_coords;
    bool coords_changed = detectCoordsChangeAngGetRobotFacing(vector_curr_coords, vector_before_coords, robot_facing,
                                                              msg_type_value);
    actions_todo = getMovesForRobot(robot_facing, coords_changed, msg_type_value.at("CLIENT_LAST_ACTION"),
                                    vector_curr_coords, vector_before_coords);
    showQ(robot_facing, actions_todo);
    if (on_zero_zero) {
        present_received = true;
        string message_to_send = "105 GET MESSAGE\a\b";
        if (sendMsg(c, message_to_send.data()) != FINE)
            return BAD;
        return FINE;
    }
    if (sendActionToRobot(c, actions_todo, msg_type_value, robot_facing) != FINE)
        return BAD;
    return FINE;
}

/**
 * Updates the current and previous coordinates of the robot.
 *
 * @param vector_curr_coords The current coordinates of the robot.
 * @param vector_before_coords The previous coordinates of the robot.
 * @param str_curr_coords A reference to the string representing current coordinates.
 * @param msg_type_value A map storing message type and value pairs.
 * @param full_received_string The full message received from the client.
 * @param c The client socket.
 */
 void getCurrBeforeCoords(vector<int> &vector_curr_coords, vector<int> &vector_before_coords, string &str_curr_coords,
                         unordered_map<string, string> &msg_type_value, string &full_received_string, int &c) {
    vector_curr_coords = extractIntegerWords(full_received_string);
    for (const auto &coord: vector_curr_coords)
        str_curr_coords += to_string(coord) + " ";
    vector_before_coords = extractIntegerWords(msg_type_value.at("CLIENT_COORDS"));
    msg_type_value.at("CLIENT_COORDS") = str_curr_coords;
}

/**
 * Selects and receives a message from the client with a timeout.
 *
 * @param retval The return value from the select function.
 * @param c The client socket.
 * @param sockets The set of sockets to monitor.
 * @param bytes_read The number of bytes read from the socket.
 * @param timeout The timeout value for the select function.
 * @param buffer_receive The buffer to store the received message.
 * @return FINE if successful, BAD if an error occurred.
 */
 int selectAndReceiveAMessage(int &retval, int &c, fd_set &sockets, int &bytes_read,
                             struct timeval timeout, char buffer_receive[BUFFER_SIZE]) {
    struct timeval timeout_tmp = timeout;
    if (received_recharging)
        timeout_tmp.tv_sec = TIMEOUT_RECHARGING;
    retval = select(c + 1, &sockets, NULL, NULL, &timeout_tmp);
    if (retval < 0) {
        perror("Chyba v select(): ");
        close(c);
        return BAD;
    }
    if (!FD_ISSET(c, &sockets)) {
        cout << "Connection timeout!" << endl;
        close(c);
        return BAD;
    }
    bytes_read = recv(c, buffer_receive, BUFFER_SIZE - 1, 0);
    if (bytes_read <= 0) {
        perror("Chyba pri cteni ze socketu: ");
        close(c);
        return BAD;
    }
    return FINE;
}

/**
 * Converts special characters in the received buffer to escape sequences.
 *
 * @param buffer_str The string to store the converted message.
 * @param buffer_receive The received message buffer.
 * @param bytes_read The number of bytes read from the socket.
 */
 void addBackSlashesToString(string &buffer_str, char *buffer_receive, int bytes_read) {
    for (int i = 0; i < bytes_read; i++) {
        if (buffer_receive[i] == '\a') {
            buffer_str += "\\a";
        } else if (buffer_receive[i] == '\b') {
            buffer_str += "\\b";
        } else if (buffer_receive[i] == '\0') {
            buffer_str += "\\0";
        } else {
            buffer_str += buffer_receive[i];
        }
    }
    cout << buffer_str << endl;
}

/**
 * Handles different types of messages received from the client.
 *
 * @param msg_cnt The current message count.
 * @param robot_facing The current facing direction of the robot.
 * @param msg The message received from the client.
 * @param msg_type_value A map storing message type and value pairs.
 * @param c The client socket.
 * @param actions_todo The queue of actions to be performed by the robot.
 * @param break_flag A flag indicating whether the connection should be closed.
 * @param sockets The set of sockets to monitor.
 * @return FINE if successful, BAD if an error occurred.
 */
 int handleMessage(size_t &msg_cnt, string &robot_facing, string &msg, unordered_map<string, string> &msg_type_value,
                  int &c, queue<string> &actions_todo, bool &break_flag, fd_set &sockets) {
    // Handling RECHARGING message
    if (msg == "RECHARGING\\a\\b") {
        received_recharging = true;
        return FINE;
    }
    if (received_recharging) {
        if (msg != "FULL POWER\\a\\b") {
            string message_to_send = "302 LOGIC ERROR\a\b";
            sendMsg(c, message_to_send.data());
            return BAD;
        } else {
            received_recharging = false;
            return FINE;
        }
    }

    if (present_received) {
        break_flag = true;
        string message_to_send = "106 LOGOUT\a\b";
        if (sendMsg(c, message_to_send.data()) != FINE)
            return BAD;
        return FINE;
    }
    if (msg_cnt >= 0 && msg_cnt <= 2) {
        if (authorizeUser(msg, msg_cnt, msg_type_value, c) != FINE)
            return BAD;
        else return FINE;
    }

    if (msg_cnt > 2) {
        if (!actions_todo.empty()) {
            if (validateCoords(c, msg) != FINE) return BAD;
            vector<int> vector_curr_coords, vector_before_coords;
            string str_curr_coords;
            getCurrBeforeCoords(vector_curr_coords, vector_before_coords, str_curr_coords, msg_type_value, msg, c);
            bool coords_changed = detectCoordsChangeAngGetRobotFacing(vector_curr_coords, vector_before_coords,
                                                                      robot_facing, msg_type_value);
            if (vector_curr_coords[1] == 0 && !already_hit_y_zero) {
                already_hit_y_zero = true;
                actions_todo = getMovesForRobot(robot_facing, coords_changed, msg_type_value.at("CLIENT_LAST_ACTION"),
                                                vector_curr_coords, vector_before_coords);
            }
            if (on_zero_zero) {
                present_received = true;
                string message_to_send = "105 GET MESSAGE\a\b";
                if (sendMsg(c, message_to_send.data()) != FINE)
                    return BAD;
                return FINE;
            }
            showQ(robot_facing, actions_todo);
            if (sendActionToRobot(c, actions_todo, msg_type_value, robot_facing) != FINE)
                return BAD;
            return FINE;
        }
        manageRobotActivity(msg, msg_cnt, msg_type_value, c, robot_facing, actions_todo);
    }
    return FINE;
}

/**
 * Checks for optimization in processing messages to avoid unnecessary operations.
 *
 * @param msg_cnt The current message count.
 * @param c The client socket.
 * @param msg The message received from the client.
 * @return FINE if valid, BAD if an error occurred.
 */
 int checkForOptimization(size_t &msg_cnt, int &c, const string &msg) {
    if (msg_cnt == 0) {
        if (validateUsername(c, msg) != FINE)
            return BAD;
    } else if (on_zero_zero) {
        if (validateSecretMessage(c, msg) != FINE)
            return BAD;
    } else if (msg_cnt > 2) {
        if (validateCoords(c, msg) != FINE)
            return BAD;
    }
    return FINE;
}

/**
 * Receives messages from the client and processes them based on the current state.
 *
 * @param c The client socket.
 * @param retval The return value from the select function.
 * @param sockets The set of sockets to monitor.
 * @param timeout The timeout value for the select function.
 * @param break_flag A flag indicating whether the connection should be closed.
 * @param buffer_receive The buffer to store the received message.
 * @return FINE if successful, BAD if an error occurred.
 */
 int receiveMsg(int &c, int &retval, fd_set &sockets, struct timeval timeout, bool &break_flag,
               char buffer_receive[BUFFER_SIZE]) {
    unordered_map<string, string> msg_type_value;
    string full_received_string;
    size_t msg_cnt = 0;
    string robot_facing = "UNKNOWN";
    queue<string> actions_todo;

    while (true) {
        FD_ZERO(&sockets);
        FD_SET(c, &sockets);
        string buffer_data;
        int bytes_read;

        if (selectAndReceiveAMessage(retval, c, sockets, bytes_read, timeout, buffer_receive) != FINE)
            return BAD;
        buffer_receive[bytes_read] = '\0';
        string buffer_str;
        addBackSlashesToString(buffer_str, buffer_receive, bytes_read);
        full_received_string += buffer_str;

        if (full_received_string.find(final_chars) == string::npos) {
            if (checkForOptimization(msg_cnt, c, full_received_string) != FINE)
                return BAD;
            continue;
        }

        vector<string> all_messages = getAllMessages(full_received_string);
        for (auto &msg: all_messages) {
            if (checkForOptimization(msg_cnt, c, msg) != FINE)
                return BAD;
            if (handleMessage(msg_cnt, robot_facing, msg, msg_type_value, c, actions_todo, break_flag, sockets) != FINE)
                return BAD;
            if (break_flag)
                return FINE;
        }
    }
    return FINE;
}

/**
 * Main function - entry point of the server application.
 *
 * @param argc The number of command-line arguments.
 * @param argv The command-line arguments.
 * @return 0 on successful completion, otherwise a negative error code.
 */
 int main(int argc, char **argv) {

    if (argc < 2) {
        cerr << "Usage: server port" << endl;
        return -1;
    }

    int l = socket(AF_INET, SOCK_STREAM, 0);
    if (l < 0) {
        perror("Nemohu vytvorit socket: ");
        return BAD;
    }

    int port = atoi(argv[1]);
    if (port == 0) {
        cerr << "Usage: server port" << endl;
        close(l);
        return BAD;
    }

    struct sockaddr_in adresa;
    bzero(&adresa, sizeof(adresa));
    adresa.sin_family = AF_INET;
    adresa.sin_port = htons(port);
    adresa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(l, (struct sockaddr *) &adresa, sizeof(adresa)) < 0) {
        perror("Problem s bind(): ");
        close(l);
        return BAD;
    }

    if (listen(l, 10) < 0) {
        perror("Problem s listen()!");
        close(l);
        return BAD;
    }

    createKeyIdMap();

    struct sockaddr_in vzdalena_adresa;
    socklen_t velikost;

    while (true) {
        int c = accept(l, (struct sockaddr *) &vzdalena_adresa, &velikost);
        if (c < 0) {
            perror("Problem s accept()!");
            close(l);
            return BAD;
        }
        pid_t pid = fork();
        if (pid == 0) {
            /**
            * The child process inherits a reference to the listening socket from the main thread.
             * Since the child process handles a specific client, it no longer needs this reference,
            * and it should be closed to avoid unnecessary socket references.
            * The main thread retains its own reference to the listening socket.
            */
            close(l);

            struct timeval timeout;
            timeout.tv_sec = TIMEOUT;
            timeout.tv_usec = 0;

            fd_set sockets;
            bool break_flag;
            int retval;
            char buffer_receive[BUFFER_SIZE];

            while (true) {
                if (receiveMsg(c, retval, sockets, timeout, break_flag, buffer_receive) != FINE) {
                    close(c);
                    return BAD;
                }
                if (break_flag)
                    break;
                close(c);
                return 0;
            }
        }
        int status = 0;
        waitpid(0, &status, WNOHANG);

        close(c);
    }
    close(l);
    return 0;
}
