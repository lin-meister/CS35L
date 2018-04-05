The biggest problem was arguably making sense of how to efficiently manage
the computation for the pixels (width* height) through multithreading. I first
referenced the syntax for pthread_create and pthread_join to create an array of
threads and looped over them to create and join threads.

The next problem was to create a function that the thread would execute once it
is created, and pass that function to pthread_create. So I moved the
computation of the pixels in the nested loop, as well as some necessary
variables for computation such as pixel_dx, pixel_dy, subsample_dx, etc., into
a separate function and passed that function into the pthread_create function.
This caused issues with some of the variables unable of being accessed,
namely scene and scaled_color, so I declared them as global variables.

Another problem that arose was when I ran my initial code with make clean check
and looked at the output. I realized that the time commands resulted in longer
processing times as the number of threads increased, which seemed contrary to
multithreading's intent in improving performance. I learned that this was
probably caused by having all the threads do computation for all pixels
instead of splitting the pixels amongst them. I went back to my code and
edited my multithreading function, so that it would slice the width evenly over
the number of threads and run each thread for the heights in its
own width.

After that, I ran make clean check and compared the resulting 1-test.ppm with
baseline.ppm:

diff 1-test.ppm baseline.ppm

and output nothing.

=====

I used the test command to benchmark my implementation of SRT using 1, 2, 4,
and 8 threads.

time ./srt 1-test.ppm >1-test.ppm.tmp

real    0m54.196s
user    0m54.188s
sys     0m0.003s

time ./srt 2-test.ppm >2-test.ppm.tmp

real    0m26.123s
user    0m27.123s
sys     0m0.003s

time ./srt 4-test.ppm >4-test.ppm.tmp

real    0m8.540s
user    0m8.917s
sys     0m0.001s

time ./srt 8-test.ppm >8-test.ppm.tmp

real    0m3.259s
user    0m3.522s
sys     0m0.003s

As we can see, introducing more threads to handle the workload in our
implementation significantly improves the running time of the program.
