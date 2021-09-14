#include "include/stdparse.h"

static char read_buf[128];

static void buf_next_tar(_Bool number, char sep)
{
  int read_buf_index = 0;
  _Bool in_target = 0;
  _Bool is_targetpart = 0;

  int curr_char;
  do
  {
      curr_char = getchar();

      // Check if is a part of target
      is_targetpart = (
        // Numbers (int, long, double, float)
        number && (curr_char >= 48 && curr_char <= 57 || curr_char == 46 || curr_char == 44 || curr_char == 45) ||

        // Characters, limited by separator (or unprintable control characters)
        !number && curr_char != sep && curr_char >= 32 && curr_char <= 126
      );

      // Comma translation to dot
      if (number && curr_char == 44) curr_char = 46;

      // Add to read buffer
      if (is_targetpart) {
        read_buf[read_buf_index++] = curr_char;
        in_target = 1;
      }
  } while (
    (!is_targetpart && !in_target) ||
    (in_target && is_targetpart)
  );

  // Terminate
  read_buf[read_buf_index] = '\0';
}

long stdp_long()
{
    buf_next_tar(1, 0);
    return atol(read_buf);
}

double stdp_double()
{
    buf_next_tar(1, 0);
    return atof(read_buf);
}

void stdp_str(char *buf, char sep)
{
  buf_next_tar(0, sep);
  strcpy(buf, read_buf);
}

char stdp_char()
{
  char res;
  do
  {
    res = getchar();
  } 
  // Until a printable char is encountered
  while (res < 32 || res > 126);
  return res;
}