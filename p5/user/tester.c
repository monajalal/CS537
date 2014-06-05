// Do not modify this file. It will be replaced by the grading scripts
// when checking your project.

#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "fs.h"
int
main(int argc, char *argv[])
{
 int fd = open("foo", O_CREATE | O_CHECKED);
  printf(1, "fd:%d\n ", fd);
  exit();
	
	
}
