#include "randcpuid.h"

#include <dlfcn.h>

#include <stdio.h>

#include <stdbool.h>

#include <stdlib.h>

#include <errno.h>



/* Main program, which outputs N bytes of random data.  */

int

main (int argc, char **argv)

{

  /* Check arguments.  */

  bool valid = false;

  long long nbytes;

  if (argc == 2)

    {

      char *endptr;

      errno = 0;

      nbytes = strtoll (argv[1], &endptr, 10);

      if (errno)

        perror (argv[1]);

      else

        valid = !*endptr && 0 <= nbytes;

    }

  if (!valid)

    {

      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);

      return 1;

    }



  /* If there's no work to do, don't worry about which library to use.  */

  if (nbytes == 0)

    return 0;



  /* Now that we know we have work to do, arrange to use the

     appropriate library.  */

  unsigned long long (*rand64) (void);



  void *handle;

  char *err;



  if (rdrand_supported ())

    {

      printf("%s\n", "HW");

      handle = dlopen("randlibhw.so", RTLD_LAZY);



      if (!handle) {

	fputs(dlerror(), stderr);

	exit(1);

      }



      // Use dlsym to get address of symbol to access function

      rand64 = dlsym(handle, "hardware_rand64");

      if ((err = dlerror()) != NULL) {

        fputs(dlerror(), stderr);

        exit(1);

      }

    }

  else

    {

      printf("%s\n", "SW");

      handle = dlopen("randlibsw.so", RTLD_LAZY);



      if (!handle) {

        fputs(dlerror(), stderr);

        exit(1);

      }



      // Use dlsym to get address of symbol to access function

      rand64 = dlsym(handle, "software_rand64");

      if ((err = dlerror()) != NULL) {

        fputs(dlerror(), stderr);

        exit(1);

      }

    }



  int wordsize = sizeof rand64 ();

  int output_errno = 0;



  do

    {

      unsigned long long x = rand64 ();

      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;

      printf("%lld\n", x);

      nbytes -= outbytes;

    }

  while (0 < nbytes);



  if (dlclose(handle) != 0) {

    fputs(dlerror(), stderr);

    exit(1);

  }



  if (fclose (stdout) != 0)

    output_errno = errno;



  if (output_errno)

    {

      errno = output_errno;

      perror ("output");

      return 1;

    }



  return 0;

}

