#include "util.h"

#define DEFAULT_OUTPUT_PATH "./a.out.zip"

void extract(char* game_path, char* output_path)
{
  // How love2d packages its game:
  // love.exe bytes + game.zip bytes
  // Find magic number, from that point onwards write down file into seperate buffer


  FileData game_file = readFile(game_path); 

  if (game_file.len == 0)
  {
    printf("Invalid filepath cupcake\n");
    return;
  }

  // .exe magic number
  if (game_file.data[0] != 0x4d || game_file.data[1] != 0x5a)
  {
    printf("File is not an PE executable\n"); 
    return;
  }

  printf("%s size: %ld\n", game_path, game_file.len);
  
  bool zip_found = false;
  size_t zip_length = 0;
	size_t zip_startaddr = 0;
  uint8_t* zip_content = 0;

  for (size_t i = 0; i < game_file.len; i++)
  {
    // Zip file magic number check
    // This magic number happens every zip entry, so check for first one
    if (
        !zip_found &&
        game_file.data[i]   == 0x50 && 
        game_file.data[i+1] == 0x4b &&
        game_file.data[i+2] == 0x03 &&
        game_file.data[i+3] == 0x04
       )
    {
      zip_found = true;
			zip_startaddr = i;
      zip_length = game_file.len - i;
      zip_content = malloc(zip_length);

      printf("Congratulations! This game can be extracted!\n");
      printf("%s is the size of %ld, %s is size of %ld and found at offset %ld\n", game_path, game_file.len, output_path, zip_length, i);
    }

    if (zip_found == true)
    {
      zip_content[i - zip_startaddr] = game_file.data[i];
    }
  }

  if (!zip_found || zip_length == 0 || zip_content == 0)
  {
    printf("Game assets not found, bad luck\n");
    return;
  }

  printf("Finished in-memory copy, writing final file to: %s - %ld\n", output_path, zip_length);
  writeFile(output_path, zip_content, zip_length);

  free(game_file.data);
  if (zip_content != 0)
  {
    free(zip_content);
  }

  printf("Done!\n");
}

int main(int argc, char** argv)
{
  if (argc == 0)
  {
    printf("What is going on?\n");
    return 1;
  }


  if (argc == 2)
  {
    extract(argv[1], DEFAULT_OUTPUT_PATH);
  } else if (argc == 3)
  {
    extract(argv[1], argv[2]);
  } else
  {
    printf("You're using me wrong, cupcake\n");
    printf("Usage: \n");
    printf("  love-tool [love2d game path]\n");
    printf("  love-tool [love2d game path] [output path]\n\n");
    printf("By if the output path is not specified will output the extracted game to %s\n\n", DEFAULT_OUTPUT_PATH);
    return 1;
  }

  return 0;
}
