#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{

    int counter = 0;
    char output_file_name[8];
    BYTE block_buffer[512];
    int block_size = sizeof(block_buffer);
    FILE *img;

    // Validate number of arguments.
    while (argc != 2)
    {
        printf("Error: One command expected.\n");
        return 1;
    }

    // File management.
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        printf("File could not be open.\n");
        return 1;
    }

    while ((fread(block_buffer, 1, block_size, file) == block_size))
    {
        if (block_buffer[0] == 0xff && block_buffer[1] == 0xd8 && block_buffer[2] == 0xff && (block_buffer[3] & 0xf0) == 0xe0)
        {
            if (counter == 0)
            {
                sprintf(output_file_name, "%03i.jpg", counter);
                img = fopen(output_file_name, "w");
                counter++;
            }
            else
            {
                fclose(img);
                sprintf(output_file_name, "%03i.jpg", counter);
                img = fopen(output_file_name, "w");
                counter++;
            }
            fwrite(block_buffer, 1, block_size, img);
        }
        else if (counter != 0)
        {
            fwrite(block_buffer, 1, block_size, img);
        }
    }
    fclose(img);
    fclose(file);
}