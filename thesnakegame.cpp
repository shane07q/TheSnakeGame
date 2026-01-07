#include <bits/stdc++.h>
#include <windows.h> // For MS Windows only
#include <conio.h>
using namespace std;

// Function to set console text color
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Classes declaration
class Board;
class Food;
class Snake;

// Point on screen/board in terms of (x,y)
// 1-based indexing/positioning
struct position {
    int x, y;

    // Constructors 
    position() {}
    position(int a, int b) : x(a), y(b) {}
};

// Board class definition
class Board {
private:
    int height, width; // height(along y), width(along x) of Playable Board
    char symbol;
    char** field;      // char 2D-array for storing values present on Board
    vector<position> obstacles; // Store positions of obstacles

public:
    // Constructor
    Board() {
        height = 30; // initializing height of board
        width = 60;  // initializing width of board

        symbol = '*';

        // dynamic memory allocation for board
        field = new char*[height + 2];
        for (int i = 0; i < height + 2; i++)
            field[i] = new char[width + 2]; 
    }

    // Destructor
    ~Board() {
        // deallocating dynamic memory allocated previously
        for (int i = 0; i < height + 2; i++)
            delete[] field[i];

        delete[] field;
    }

    // Add obstacle at position (x, y)
    void add_obstacle(int x, int y) {
        obstacles.push_back(position(x, y));
        field[y][x] = '#'; // '#' represents an obstacle
    }

    // Generate random obstacles
    void generate_obstacles(int num_obstacles) {
        obstacles.clear(); // Clear existing obstacles
        for (int i = 0; i < num_obstacles; i++) {
            int x, y;
            do {
                x = 1 + rand() % width;
                y = 1 + rand() % height;
            } while (field[y][x] != ' '); // Ensure the position is empty
            add_obstacle(x, y);
        }
    }

    // Check if a position is an obstacle
    bool is_obstacle(int x, int y) {
        for (const auto& obs : obstacles) {
            if (obs.x == x && obs.y == y)
                return true;
        }
        return false;
    }

    // print board on screen
    void show_board() {
        cout << endl;
     
        for (int i = 0; i < height + 2; i++) {
            for (int j = 0; j < width + 2; j++) {
                if (i == 0 || i == height + 1 || j == 0 || j == width + 1) {
                    setColor(10); // Green border
                    cout << field[i][j];
                    setColor(7); // Reset to default color
                } else if (field[i][j] == '#') {
                    setColor(9);
                    cout << field[i][j];
                    setColor(7);
                } else {
                    cout << field[i][j];
                }
            }
            cout << endl;
        }
    }

    // clean the board
    void clear_board() {
        for (int i = 0; i < height + 2; i++) {
            for (int j = 0; j < width + 2; j++)
                field[i][j] = ' ';
        }

        // set boundary
        for (int i = 0; i < width + 2; i++)
            field[0][i] = symbol;
        for (int i = 0; i < width + 2; i++)
            field[height + 1][i] = symbol;
        for (int i = 0; i < height + 2; i++)
            field[i][0] = symbol;
        for (int i = 0; i < height + 2; i++)
            field[i][width + 1] = symbol;

        // Re-add obstacles after clearing the board
        for (const auto& obs : obstacles) {
            field[obs.y][obs.x] = '#';
        }
    }

    // returns board height
    int get_board_height() {
        return height;
    }

    // returns board width
    int get_board_width() {
        return width;
    }

    // set value on board
    void set_on_board(int row, int col, char c) {
        field[row][col] = c;
    }

    // return boundary symbol
    char get_boundary_symbol() {
        return symbol;
    }

} field; // object with name "field" of class "Board"

// Food class definition
class Food {
private:
    position food_point; // position of food on the board
    char symbol;         // character symbol for representing food

public:
    // Constructor
    Food() : symbol('X') {}

    // set food on board when given food's coordinates
    void set_food_position(int x, int y, Board& field) {
        food_point.x = x;
        food_point.y = y;
    }

    // randomly allocating food position on board with rand() function
    void reset_food_position(Board& field) {
        do {
            food_point.x = 1 + rand() % field.get_board_width();
            food_point.y = 1 + rand() % field.get_board_height();
        } while (field.is_obstacle(food_point.x, food_point.y)); // Ensure food doesn't spawn on an obstacle
    }

    // returns x-coordinate of food
    int get_food_x() {
        return food_point.x;
    }

    // returns y-coordinate of food
    int get_food_y() {
        return food_point.y;
    }

    // return food symbol
    char get_food_symbol() {
        return symbol;
    }

} eat;

// Snake class definition
class Snake {
private:
    char body_head_symbol, body_part_symbol; // symbols for head and body(except) head of snake
    vector<position> body;                   // storing body of snake body[0]->head, body[size-1]->tail
    int body_size;                           // size of snake's body
    position head;                           // position of snake's head on board
    enum direction { UP, DOWN, LEFT, RIGHT }; // directions of motion(w.r.t player in front of screen)
    direction dir;                           // current direction of motion of snake's head
    int level;                               // current level of the game

public:
    // Constructor
    Snake(int x = 15, int y = 15) : body_head_symbol('@'), body_part_symbol('o'), dir(DOWN), body_size(3), level(1) {
        // Set initial body positions
        for (int i = 0; i < body_size; i++) {
            position tmp(x, y - i); // Snake starts vertically downwards
            body.push_back(tmp);
        }

        head = body[0]; // Set head position
    }

    // returns x-coordinate of head
    int get_head_x() {
        return head.x;
    }

    // returns y-coordinate of head
    int get_head_y() {
        return head.y;
    }

    // returns symbol of snake's head
    char get_head_symbol() {
        return body_head_symbol;
    }

    // returns symbol of snake's non-head
    char get_nonhead_symbol() {
        return body_part_symbol;
    }

    // gets player input for direction of head and store in dir
    void get_input() {
        if ((GetAsyncKeyState(VK_UP) || (GetAsyncKeyState('W')))) {
            if (dir != DOWN) dir = UP;
        } else if ((GetAsyncKeyState(VK_DOWN) || (GetAsyncKeyState('S')))) {
            if (dir != UP) dir = DOWN;
        } else if ((GetAsyncKeyState(VK_LEFT) || (GetAsyncKeyState('A')))) {
            if (dir != RIGHT) dir = LEFT;
        } else if ((GetAsyncKeyState(VK_RIGHT) || (GetAsyncKeyState('D')))) {
            if (dir != LEFT) dir = RIGHT;
        }
    }

    // movement of snake
    void move() {
        position head_modify(0, 0);
        if (dir == UP)
            head_modify.y = -1;
        else if (dir == DOWN)
            head_modify.y = 1;
        else if (dir == RIGHT)
            head_modify.x = 1;
        else if (dir == LEFT)
            head_modify.x = -1;

        position new_head(0, 0);
        new_head.x = body[0].x + head_modify.x;
        new_head.y = body[0].y + head_modify.y;

        // Check for obstacle collision
        if (field.is_obstacle(new_head.x, new_head.y)) {
            throw (string)"SNAKE HIT AN OBSTACLE.....!!!!";
        }

        // Update snake position
        for (int i = body_size - 1; i > 0; i--)
            body[i] = body[i - 1];
        head = new_head;
        body[0] = head;

        // Check for collision with body
        for (int i = 1; i < body_size; i++) {
            if (body[i].x == head.x && body[i].y == head.y) {
                throw (string)"SNAKE DEAD.....!!!!";
            }
        }

        // Check for wall collision
        if (head.x <= 0 || head.y <= 0 || head.x >= 1 + field.get_board_width() || head.y >= 1 + field.get_board_height()) {
            throw (string)"SNAKE DEAD.....!!!!";
        }
    }

    // tells if snake(head) has reached food
    bool food_found(Food& foodie) {
        if (foodie.get_food_x() == head.x && foodie.get_food_y() == head.y) {
            // size increment
            body_size += 1;

            // adds a temporary position at end which is helpful while movement of snake(move method) after eating
            position tmp(0, 0);
            body.push_back(tmp);

            // Check if the snake has grown enough to level up
            if (body_size % 5 == 0) { // Level up every 5 foods
                level++;
                field.generate_obstacles(5 + level * 2); // Increase obstacles with level
                cout << "Level Up! Current Level: " << level << endl;
            }
            return true;
        }
        return false;
    }

    // set the snake's symbols on board at it's position
    void set_snake_onboard(Board& draw_here) {
        setColor(12); // Red head
        field.set_on_board(head.y, head.x, body_head_symbol);
        setColor(7); // Reset to default color

        setColor(14); // Yellow body
        for (int i = 1; i < body.size(); i++)
            field.set_on_board(body[i].y, body[i].x, body_part_symbol);
        setColor(7); // Reset to default color
    }

    // returns the current score (body size - 1)
    int get_score() {
        return body_size - 3;
    }

    // returns the current level
    int get_level() {
        return level;
    }

} player; // object "player" of class "Snake"

// Hide cursor function only in windowed mode (i.e not full screen)
void hide_cursor() {
    HANDLE hStdOut = NULL;
    CONSOLE_CURSOR_INFO curInfo;

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &curInfo);
}

// Function to set cursor position on console
void set_cursor_position(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to display the game title
void show_title() {
    setColor(11); // Cyan color
    cout << "==============================================" << endl;
    cout << "|               THE SNAKE GAME              |" << endl;
    cout << "==============================================" << endl;
    cout << endl;
    setColor(7); // Reset to default color
}

// Function to display the player's name in a box
void show_player_name(const string& name) {
    setColor(13); // Magenta color
    cout << "+---------------------+" << endl;
    cout << "| Player: " << setw(12) << left << name << " |" << endl;
    cout << "+---------------------+" << endl;
    setColor(7); // Reset to default color
}

// Function to write scores to a file
void write_scores_to_file(const string& player_name, int score, int high_score) {
    ofstream outFile("scores.txt", ios::app); // Open file in append mode
    if (outFile.is_open()) {
        outFile << "Player: " << player_name << " | Score: " << score << " | High Score: " << high_score << endl;
        outFile.close();
    } else {
        cerr << "Unable to open file to write scores!" << endl;
    }
}

// Function to read the high score for a particular player
int read_high_score(const string& player_name) {
    ifstream inFile("scores.txt");
    int high_score = 0;
    string line;

    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            size_t pos = line.find("Player: " + player_name);
            if (pos != string::npos) {
                // Extract the high score from the line
                size_t score_pos = line.find("High Score: ");
                if (score_pos != string::npos) {
                    int score = stoi(line.substr(score_pos + 12));
                    if (score > high_score) {
                        high_score = score;
                    }
                }
            }
        }
        inFile.close();
    } else {
        cerr << "Unable to open file to read scores!" << endl;
    }

    return high_score;
}

// Function to handle pause functionality
void pause_game() {
    cout << "Game Paused! Press 'R' to Resume..." << endl;
    while (true) {
        if (_kbhit()) { // Check if a key is pressed
            char key = _getch(); // Get the pressed key
            if (key == 'R' || key == 'r') { // Resume on 'R' key
                cout << "Game Resumed!" << endl;
                break;
            }
        }
    }
}

// Main function
int main() {
    string player_name;
    int high_score = 0;
    int difficulty;

    // Display game title
    show_title();

    cout << "Enter your name: ";
    cin >> player_name;

    // Read the high score for the player
    high_score = read_high_score(player_name);
    cout << "Your current high score: " << high_score << endl;

    cout << "Choose difficulty level (1: Beginner, 2: Intermediate, 3: Advanced): ";
    cin >> difficulty;

    int sleep_time;
    switch (difficulty) {
        case 1:
            sleep_time = 100;
            break;
        case 2:
            sleep_time = 60;
            break;
        case 3:
            sleep_time = 30;
            break;
        default:
            sleep_time = 60;
            break;
    }

    // Hides cursor
    hide_cursor();

    // Current unix time in seconds as seed for rand function
    srand(time(0));

    // Generate initial obstacles
    field.generate_obstacles(5); // Start with 5 obstacles

    // Sets initial food position
    eat.reset_food_position(field);

    bool is_paused = false; // Track pause state
    
    // Loop until the player kills snake
    while (1) {
        if (!is_paused) {
            field.clear_board(); // Clears board
            player.get_input();  // Finds if user has pressed any key until previous execution of loop

            // Moves snake
            try {
                player.move();
            }
            catch (string err) {
                field.clear_board();
                cout << err << endl;
                cout << "Player: " << player_name << endl;
                cout << "Score: " << player.get_score() << endl;
                cout << "Level: " << player.get_level() << endl;
                if (player.get_score() > high_score) {
                    high_score = player.get_score();
                    cout << "New High Score!" << endl;
                }
                cout << "High Score: " << high_score << endl;

                // Write scores to file
                write_scores_to_file(player_name, player.get_score(), high_score);

                system("pause"); // Pause system and wait for key press, MS Windows (NOT Linux)
                return 0;
            }

            // Set food on board with color
            setColor(10); // Green food
            field.set_on_board(eat.get_food_y(), eat.get_food_x(), eat.get_food_symbol());
            setColor(7); // Reset to default color

            player.set_snake_onboard(field); // Set snake on board

            // If snake(head) has found food, reset food randomly
            if (player.food_found(eat)) {
                eat.reset_food_position(field);
            }

            // Display the board and score
            set_cursor_position(0, 4);     // Move cursor down before printing player name
            show_player_name(player_name); // Display player's name
            set_cursor_position(0, 6);     // Move cursor further down before printing the board
            field.show_board();            // Prints board
            cout << "Score: " << player.get_score() << endl;
            cout << "Level: " << player.get_level() << endl;
            cout << "High Score: " << high_score << endl;
        }

        // Check for pause key (e.g., 'P')
        if (_kbhit()) {
            char key = _getch();
            if (key == 'p') { // Pause on 'p' key
                is_paused = true;
                pause_game(); 
                is_paused = false;
            }
        }

        Sleep(sleep_time); // Windows.h --> milliseconds for which to stop execution
    }

    return 0;
}