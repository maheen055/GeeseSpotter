#include "geesespotter_lib.h"
#include <iostream>

char *create_board(std::size_t x_dim, std::size_t y_dim){
    ///calculate the total size of the array
    std::size_t total_size = x_dim*y_dim;
    ///allocate memory for the char array and initialize with the value 0, not char '0', as stated
    char* board = new char[total_size];

    for (std::size_t index = 0; index< total_size; index++){
        board[index]=0; ///fill array with null using for loop::: ask prof harder during office hours if this is needed
    }

    return board;
}

void clean_board(char *board){
    delete[] board; ///referenced from "dynamic allocation of arrays"
    board=nullptr; ///in memory, destroys the stored value
}

void print_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for (std::size_t y=0; y<y_dim;y++){
        for (std::size_t x=0; x<x_dim; x++){
            ///using the x and y co-or., calculate the index in the 1d array
            std::size_t index = y*x_dim+x;
            char field = board [index];

            ///0x10 is 00010000 in binary
            ///& performs bitwise and operation (checks if the fifth bit is set)
            ///if it is set, field is marked (print 'M')
            if ((field & marked_mask())!=0){
                std::cout<<'M'; 
            } else if((field & hidden_mask())!=0){
                ///0x20 is 00100000
                ///check if the sixth bit is set (is it 1?), meaning it is hidden --> print 'h'
                std::cout<<'*';
            } else if (field ==9){
                std::cout<<'9';///tile contains goose
            } else{
                std::cout<<((char)(field+'0'));///0 to 8
            }
        }
        std::cout << std::endl;//this was needed for new row
    }
}

void hide_board(char *board, std::size_t x_dim, std::size_t y_dim){
    for (int index=0; index<x_dim*y_dim; index++){
        ///now we just set the hidden bit in the field
        board[index] |= hidden_mask();//we use or bc we are preserving the rest of the bit values
    }
}

int mark(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,std::size_t y_loc){
    std::size_t index = y_loc*x_dim+x_loc;
    ///check if hidden bit is set
    if (board[index] &hidden_mask()){
        board[index] ^= marked_mask();

        return 0;
    } else{
        return 2;
    }
}

void compute_neighbours(char *board, std::size_t x_dim, std::size_t y_dim) {
    int n[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int m[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (std::size_t y = 0; y < y_dim; y++) {
        for (std::size_t x = 0; x < x_dim; x++) {
            std::size_t index = y * x_dim + x;
            if (board[index] != 9) {
                int geese_count = 0;
                for (int n_index = 0; n_index < 8; n_index++) {
                    int new_x = x + n[n_index];
                    int new_y = y + m[n_index];

                    if (new_x >= 0 && new_x < x_dim && new_y >= 0 && new_y < y_dim) {
                        std::size_t neighbor_index = new_y * x_dim + new_x;
                        if (board[neighbor_index] == 9) {
                            geese_count++;
                        }
                    }
                }
                board[index] = static_cast<char>(geese_count);
            }
        }
    }
}

bool is_game_won(char *board, std::size_t x_dim, std::size_t y_dim) {
    int goose_count = 0;

    for (std::size_t y = 0; y < y_dim; y++) {
        for (std::size_t x = 0; x < x_dim; x++) {
            std::size_t index = y * x_dim + x;

            if (((board[index] & value_mask()) != 9) && ((board[index] & (hidden_mask() | marked_mask())) != 0)) {
                return false;
            } else if ((board[index] & value_mask()) == 9) {
                goose_count++;
            }
        }
    }

    return goose_count == x_dim * y_dim;
}


int reveal(char *board, std::size_t x_dim, std::size_t y_dim, std::size_t x_loc,std::size_t y_loc){
    std::size_t index = y_loc*x_dim+x_loc;

    if ((board[index] & marked_mask()) !=0){
        return 1; ///tile is marked, return 1 without modifications
    } else if(((board[index] & hidden_mask()) >=0) && (board[index] & hidden_mask()) <=8){
        return 2; ///tile is revealed, return 2
    } else if((board[index] & value_mask())==9){///if theres a goose, reveal and return 9
        board[index] &= ~hidden_mask(); //clear hidden
        return 9;
    } else{///tile is hidden and unmarked, reveal it
        board[index] &= ~hidden_mask(); //clear hidden

        if ((board[index] & value_mask())==0){///if revealed tile is empty, reveal neighbouring fields (upto 9 adjacent tiles)
            for (int y=-1; y<=1;y++){
                for (int x=-1;x<=1; x++){
                    int new_x = x_loc+x;
                    int new_y = y_loc+y;

                    if (new_x>=0 && new_x < static_cast<int>(x_dim) && new_y>=0 && new_y < static_cast<int>(y_dim)){
                        std::size_t new_index = static_cast<std::size_t>(new_y)*x_dim+static_cast<std::size_t>(new_x);
                        if ((board[new_index] & marked_mask())==0){///reveal neighbouring field if not marked
                            board[new_index] &= ~hidden_mask();
                        }
                    }
                }
            }
        }
        return 0; /// all have been revealed 
    }
}