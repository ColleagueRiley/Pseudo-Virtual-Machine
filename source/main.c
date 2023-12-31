/* for cross compiling */
#ifdef _WIN32
#undef __linux__
#undef __unix__
#endif

#define RGFW_BUFFER
#define RGFW_IMPLEMENTATION

#define SI_IMPLEMENTATION

#include <stdbool.h>

#include "RGFW.h" /* simple multi-platform abstaction layer for creaing/managing windows */
#include "sili.h" /* standard toolchain stuff */
#include "draw.h" /* drawPixel and drawRect */

bool running = true;

unsigned char memory[4096];
unsigned char registers[16]; /* V0 - VF */

/* note : 
    VF is being used for controller input
*/

int main (int argc, char** argv) {
    si_init(SI_KILO(500));

    srand(time(NULL));

    if (argc == 1) {
        printf("A rom file is required\n");
        return 1;
    }

    /* make sure there the rom is real */
    if (!si_path_exists(argv[1])) {
        printf("Rom file %s does not exist\n", argv[1]);
        return 1;
    }
    
    /* open rom and read it into a string */
    siFile f = si_file_open(argv[1]);
    siString filestr = si_file_read(f);

    si_file_close(f); /* close the file because we don't need it anymore */

    /* copy file string into RAM */
    int i; 
    for (i = 0; i < si_string_len(filestr); i++)
        memory[i] = filestr[i];

    RGFW_window* window = RGFW_createWindowPointer("Pseudo VM", 0, 0, 100, 100, RGFW_NO_RESIZE);

    int* screenSize = RGFW_getScreenSize(window);

    window->x = (screenSize[0] + 100) / 2;
    window->y = (screenSize[1] + 100) / 4;

    int N, X; /* vars [for checking bool for a while loop] */
    bool ifBool = true; /* bool for an if statement */
    
    int whileStart = -1; /* where the while loop starts (-1 means we're not in a while loop)  */
    int whileType = 0;

    clearScreen(window);

    for (i = 0; i < 4096 && running; i++) {

        /*
        KEY

        V -> register [VX = register[x]]
        X -> register number (0 - F)
        N -> number, 0 - 99
              numbers must be represented using two digits
              ex. 9 -> 09
        M -> memory address from 0 - 4096
                number bust be represented using unicode value
                ex. 65 -> A
                ex. 

                * memory lower than 1500 is locked for the RAM,
                    this means 1500 is added to whatever RAM address you're trying to access

        *****opcodes****** 
        ==================
        0000   * close program or end while loop ( supported )
        1FXN   * if N == VX [do next line] ( supported )
        1AXN   * while N == VX ( supported )
        1BXN   * if N >= VX [do next line]
        1CXN   * while N >= VX 
        1DXN   * if N <= VX [do next line]
        1EXN   * while N <= VX
        A1XN   * generates a random number from 0 - N into VX
        C000   * clear screen ( supported )
        DFXM   * dumps value at RAM address M into VX ( supported )
        DSXY   * sets V(S) to X and V(S + 1) to Y ( supported )
        DESX   * draws rectangle at (V(X), V(X + 1)) with w width of V(X + 3) and a height of V(X + 4)
        EENM   * memory[M] = N sets the value at RAM address M to N ( supported )
        EANM   * memory[M] += N adds N to the value at RAM address M
        EFXM   * memory[M] += registers[X] adds the value at register X to the value at RAM address M
        
        ==================
        */

        switch (memory[i]) {
            case '0':
                /* 0000 */
                if (memory[i + 1] == '0' && memory[i + 2] == '0' && memory[i + 3] == '0') { 
                    if (whileStart == -1)
                        running = false;
                    else if ( (whileType == 0 && N == registers[X]) ||
                              (whileType == 1 && N >= registers[X]) ||
                              (whileType == 2 && N <= registers[X])
                            )
                            i = whileStart;
                    else
                        whileStart = -1;
                }

                break;
            case '1':
                i++;

                if (memory[i] == 'F') {
                    i++;
                    int X = registers[si_hex_digit_to_int(memory[i])];
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    int N = si_cstr_to_i64(str);

                    if (X != N)
                        i += 3;
                }
                
                else if (memory[i] == 'A') {
                    whileStart = i + 1;

                    i++;
                    
                    X = si_hex_digit_to_int(memory[i]);
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    N = si_cstr_to_i64(str);

                    whileType = 0;
                }

                if (memory[i] == 'B') {
                    i++;
                    int X = registers[si_hex_digit_to_int(memory[i])];
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    int N = si_cstr_to_i64(str);

                    if (X < N)
                        i += 3;
                }
                
                else if (memory[i] == 'C') {
                    whileStart = i + 1;

                    i++;
                    
                    X = si_hex_digit_to_int(memory[i]);
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    N = si_cstr_to_i64(str);

                    whileType = 1;
                }

                else if (memory[i] == 'E') {
                    i++;
                    int X = registers[si_hex_digit_to_int(memory[i])];
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    int N = si_cstr_to_i64(str);

                    if (X > N)
                        i += 3;
                }

                else if (memory[i] == 'C') {
                    whileStart = i + 1;

                    i++;
                    
                    X = si_hex_digit_to_int(memory[i]);
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    N = si_cstr_to_i64(str);

                    whileType = 2;
                }

                break;

            case 'D':
                i++;
                
                if (memory[i] == 'F') { 
                    i++;
                    
                    registers[si_hex_digit_to_int(memory[i])] = memory[memory[i + 1]];

                }

                else if (memory[i] == 'E' && memory[i + 1] == 'S') {
                    i += 2;

                    int X = si_hex_digit_to_int(memory[i]);
                    
                    drawRect(window, registers[X + 1], registers[X], registers[X + 2], registers[X + 3], (char[]){255, 255, 255});
                }

                else {
                    int X = si_hex_digit_to_int(memory[i]);
                    i++;
                    
                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    str[3] = '\0';

                    int x = si_cstr_to_i64(str);

                    char str2[3] = {memory[i + 2], memory[i + 3], '\0'};
                    int y = si_cstr_to_i64(str2);

                    registers[X] = x;
                    registers[X + 1] = y;
                }

                break;

            case 'E':
                i++;
                
                if (memory[i] == 'E') {
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    i++;

                    memory[memory[i + 1]] = si_cstr_to_i64(str);
                }
                
                else if (memory[i] == 'A') {
                    i++;

                    char str[3] = {memory[i], memory[i + 1], '\0'};
                    i++;

                    memory[memory[i + 1]] += si_cstr_to_i64(str);
                }

                else if (memory[i] == 'F') {
                    i++;   


                    memory[memory[i + 1]] += registers[si_hex_digit_to_int(memory[i])];
                }

                break;

            case 'A':
                /* A1XN */
                i++;
                if (memory[i] == '1') {
                    i++;

                    char str[3] = {memory[i + 1], memory[i + 2], '\0'};

                    registers[si_hex_digit_to_int(memory[i])] = rand() % si_cstr_to_i64(str);
                }

                break;

            case 'C':
                /* C000 */
                if (memory[i + 1] == '0' && memory[i + 2] == '0' && memory[i + 3] == '0')
                    clearScreen(window);

                break;
            default: break;
        }
        
        RGFW_checkEvents(window);

        switch (window->event.type) {
            case RGFW_quit: 
                running = false;
                break;

            case RGFW_keyPressed:
                if (si_cstr_equal(window->event.keyName, "Up"))
                    registers[15] = 1;
                else if (si_cstr_equal(window->event.keyName, "Down"))
                    registers[15] = 2;
                else if (si_cstr_equal(window->event.keyName, "Left"))
                    registers[15] = 3;
                else if (si_cstr_equal(window->event.keyName, "Right"))
                    registers[15] = 4;
                else if (si_cstr_equal(window->event.keyName, "z"))
                    registers[15] = 5;
                else if (si_cstr_equal(window->event.keyName, "x"))
                    registers[15] = 6;
                
                break;
            case RGFW_keyReleased:
                /* 
                    makes sure the key released matches the one the register says is pressed before we set the register to 0 
                    otherwise this causes the controls to act up or stop when another key/button is released
                */
                if (si_cstr_equal(window->event.keyName, "Up") && registers[15] == 1
                   || si_cstr_equal(window->event.keyName, "Down") && registers[15] == 2
                   || si_cstr_equal(window->event.keyName, "Left") && registers[15] == 3
                   || si_cstr_equal(window->event.keyName, "Right") && registers[15] == 4
                   || si_cstr_equal(window->event.keyName, "z") && registers[15] == 5
                   || si_cstr_equal(window->event.keyName, "x") && registers[15] == 6)
                        registers[15] = 0;
                break;
        }
    }

    RGFW_closeWindow(window);

    si_terminate();
}