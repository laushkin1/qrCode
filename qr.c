#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void encode_char(const char character, bool bits[8]);
char decode_byte(const bool bits[8]);
void encode_string(const char string[], bool bytes[strlen(string)+1][8]);
void decode_bytes(const int rows, bool bytes[rows][8], char string[rows]);
void bytes_to_blocks(const int cols, const int offset, bool blocks[offset*8][cols], const int rows, bool bytes[rows][8]);
void blocks_to_bytes(const int cols, const int offset, bool blocks[offset*8][cols], const int rows, bool bytes[rows][8]);

int main(){
    // # 1
    bool bits1[8];
    encode_char('A', bits1);
    for(int i = 0; i < 8; i++){
        printf("%d", bits1[i]);
    }
    printf("\n");
    // prints: 01000001

    bool bits2[8] = {0,1,0,0,0,0,0,1};
    printf("%c\n", decode_byte(bits2));
    // prints: A


    // # 2
    char* text = "Hello, how are you?";
    const int len = strlen(text);
    bool bytes1[len+1][8];
    encode_string(text, bytes1);
    for(int j = 0; j <= len; j++){
        printf("%c: ", text[j]);
        for(int i = 0; i < 8; i++){
            printf("%d", bytes1[j][i]);
        }
        printf("\n");
    }
    // prints:
    // H: 01001000
    // e: 01100101
    // l: 01101100
    // l: 01101100
    // o: 01101111
    // ,: 00101100
    //  : 00100000
    // h: 01101000
    // o: 01101111
    // w: 01110111
    //  : 00100000
    // a: 01100001
    // r: 01110010
    // e: 01100101
    //  : 00100000
    // y: 01111001
    // o: 01101111
    // u: 01110101
    // ?: 00111111
    // : 00000000

    bool bytes2[7][8] = {
            {0,1,0,0,1,0,0,0},
            {0,1,1,0,0,1,0,1},
            {0,1,1,0,1,1,0,0},
            {0,1,1,0,1,1,0,0},
            {0,1,1,0,1,1,1,1},
            {0,0,1,0,0,0,0,1},
            {0,0,0,0,0,0,0,0}
    };
    char string[7];
    decode_bytes(7, bytes2, string);
    printf("%s\n", string);
    // prints: Hello!


    // # 3
    int length = 4+1, cols = 3, offset = 2;
    bool bytes1_[4+1][8] = {
            {0,1,0,0,0,0,0,1},
            {0,1,1,0,1,0,0,0},
            {0,1,1,0,1,1,1,1},
            {0,1,1,0,1,0,1,0},
            {0,0,0,0,0,0,0,0}
    };
    bool blocks1[offset*8][cols];
    bytes_to_blocks(cols, offset, blocks1, length, bytes1_);
    for(int j = 0; j < offset*8; j++){
        for(int i = 0; i < cols; i++){
            printf("%d ", (blocks1[j][i] == true) ? 1 : 0);
        }
        printf("\n");
        if(j % 8 == 7){
            printf("\n");
        }
    }
    // prints:
    // 0 0 0
    // 1 1 1
    // 0 1 1
    // 0 0 0
    // 0 1 1
    // 0 0 1
    // 0 0 1
    // 1 0 1
    //
    // 0 0 0
    // 1 0 0
    // 1 0 0
    // 0 0 0
    // 1 0 0
    // 0 0 0
    // 1 0 0
    // 0 0 0

    bool blocks2[2*8][3] = {
            {0,0,0},
            {1,1,1},
            {0,1,1},
            {0,0,0},
            {0,1,1},
            {0,0,1},
            {0,0,1},
            {1,0,1},
            {0,0,0},
            {1,0,0},
            {1,0,0},
            {0,0,0},
            {1,0,0},
            {0,0,0},
            {1,0,0},
            {0,0,0}
    };
    bool bytes2_[length][8];
    blocks_to_bytes(3, 2, blocks2, length, bytes2_);
    for(int j = 0; j < length; j++){
        for(int i = 0; i < 8; i++){
            printf("%d", bytes2_[j][i]);
        }
        printf("\n");
    }
    // prints:
    // 01000001
    // 01101000
    // 01101111
    // 01101010
    // 00000000
    return 0;
}

void encode_char(const char character, bool bits[8]){
    int ascii_char = character;
    for (int i=7; i>=0; i--){
        if(ascii_char == 0){
            bits[0] = 0;
            bits[1] = 0;
            bits[2] = 0;
            bits[3] = 0;
            bits[4] = 0;
            bits[5] = 0;
            bits[6] = 0;
            bits[7] = 0;
            return;
        }
        int bit = (ascii_char >> i) & 1;
        bits[7-i] = bit;
    }
}

char decode_byte(const bool bits[8]){
    int ascii_char = 0;
    for (int i = 0; i < 8; i++) {
        ascii_char = (ascii_char << 1) | bits[i];
    }
    return ascii_char;
}

void encode_string(const char string[], bool bytes[strlen(string)+1][8]){
    for (int i = 0; i <= strlen(string); i++){
        encode_char(string[i], bytes[i]);
    }
}

void decode_bytes(const int rows, bool bytes[rows][8], char string[rows]){
    for (int i = 0; i < rows; i++){
        string[i] = decode_byte(bytes[i]);
    }
}

void bytes_to_blocks(const int cols, const int offset, bool blocks[offset*8][cols], const int rows, bool bytes[rows][8]){

    // add last 00000000 cols to transp_matrix ('\0')
    int rows_ = rows;
    while(offset * cols != rows_){
        rows_++;
    }
    char transp_matrix[8][rows_];

    // make all transp_matrix to 0
    for (int b = 0; b < 8; b++){
        for (int r = 0; r < rows_; r++){
            transp_matrix[b][r] = 0;
        }
    }

    // make all blocks to 0
    for(int o = 0; o < offset*8; o++) {
        for (int c = 0; c < cols; c++) {
            blocks[o][c] = 0;
        }
    }

    // fill transp_matrix
    for (int r = 0; r < rows; r++) {
        for (int b = 0; b < 8; b++) {
            transp_matrix[b][r] = bytes[r][b];
        }
    }


    // fill blocks
    for(int o = 0; o < offset*8; o++){
        for (int c = 0; c < cols; c++) {
            if(o<8){
                blocks[o][c] = transp_matrix[o][c];
            } else if(o>=8 && o<16){
                blocks[o][c] = transp_matrix[o-8][c+cols];
            } else if (o>=16 && o<24){
                blocks[o][c] = transp_matrix[o-16][c+cols*2];
            } else if(o>=24 && o<32){
                blocks[o][c] = transp_matrix[o-24][c+cols*3];
            } else if(o>=32 && o<40){
                blocks[o][c] = transp_matrix[o-32][c+cols*4];
            } else if(o>=40 && o<48){
                blocks[o][c] = transp_matrix[o-40][c+cols*5];
            } else if(o>=48 && o<56){
                blocks[o][c] = transp_matrix[o-48][c+cols*6];
            } else if(o>=56 && o<64){
                blocks[o][c] = transp_matrix[o-56][c+cols*7];
            } else if(o>=64 && o<72){
                blocks[o][c] = transp_matrix[o-64][c+cols*8];
            } else if(o>=72 && o<80){
                blocks[o][c] = transp_matrix[o-72][c+cols*9];
            }

        }
    }
}

void blocks_to_bytes(const int cols, const int offset, bool blocks[offset*8][cols], const int rows, bool bytes[rows][8]){

    for(int r = 0; r < rows; r++) {
        for (int c = 0; c < 8; c++) {
            bytes[r][c] = 0;
        }
    }

    for(int o = 0; o < offset*8; o++){
        for (int c = 0; c < cols; c++){
            if(o<8) {
                bytes[c][o] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=8 && o<16){
                bytes[c+cols][o-8] = (blocks[o][c] == true) ? 1 : 0;
            } else if (o>=16 && o<24){
                bytes[c+cols*2][o-16] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=24 && o<32){
                bytes[c+cols*3][o-24] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=32 && o<40){
                bytes[c+cols*4][o-32] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=40 && o<48){
                bytes[c+cols*5][o-40] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=48 && o<56){
                bytes[c+18][o-48] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=56 && o<64){
                bytes[c+21][o-56] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=64 && o<72){
                bytes[c+24][o-64] = (blocks[o][c] == true) ? 1 : 0;
            } else if(o>=72 && o<80){
                bytes[c+27][o-72] = (blocks[o][c] == true) ? 1 : 0;
            }
        }
    }
}
