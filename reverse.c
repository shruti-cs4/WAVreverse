#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

const int hedSize = 44;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Read header
    // TODO #3
    WAVHEADER hed;
    fread(&hed, hedSize, 1, input);

    // Use check_format to ensure WAV format
    // TODO #4
    if (!check_format(hed))
    {
        printf("Input is not wav file.\n");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("could not open file.\n");
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&hed, hedSize, 1, output);

    // Use get_block_size to calculate size of block
    // TODO #7
    int blockSize = get_block_size(hed);

    // Write reversed audio to file
    // TODO #8
    uint16_t *data = (uint16_t *) malloc(blockSize);
    fseek(input, 0, SEEK_END);
    long fileSize = ftell(input);
    long dataStart = hedSize;
    int numBlocks = (fileSize - dataStart) / blockSize;
    fseek(input, dataStart, SEEK_SET);

    for (int j = 0; j < numBlocks; j++)
    {
        fread(data, blockSize, 1, input);
        for (int i = 0; i < blockSize / 2; i++)
        {
            uint16_t temporary = data[i];
            data[i] = data[blockSize - i - 1];
            data[blockSize - i - 1] = temporary;
        }
        fwrite(data, blockSize, 1, output);
    }

    free(data);
    fclose(input);
    fclose(output);
}

int check_format(WAVHEADER header)
{
    if (memcmp(header.format, "WAVE", 4) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int get_block_size(WAVHEADER header)
{
    // TODO #7
    int size = (header.bitsPerSample / 8) * header.numChannels;
    return size;
}