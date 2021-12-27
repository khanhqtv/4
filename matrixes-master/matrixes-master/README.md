# matrixes
C++ Console app that creates and manages matrixes. Practices in C++ coding vulnerabilities
## postgres
install libpqxx to comunicate the application with Postgresql http://pqxx.org/development/libpqxx/

From Synaptic Package Manager install postgres, libp, and lbpqxx

##libsodium
Downlaod current tar from https://download.libsodium.org/libsodium/releases/
then:
>$ ./configure

>$ make && make check

>su make install

You need to run ldconfig after installing a new library.

If you get error like libsodium.so.18: cannot open shared object file: No such file or directory
You can execute 
>cp /usr/local/lib/libsodium.so.18 /usr/lib

To compile: 
>g++ src/main.cpp -I /usr/local/include/pqxx -I include/ -o build/main -lpqxx -lpq -lsodium

and then 

>./build/main

Login Screen:<br/>

![ScreenShot](https://cloud.githubusercontent.com/assets/3706122/18183600/6a662d3c-7095-11e6-91ba-923fd788ef34.png)

Initial menu:<br/>

![ScreenShot](https://cloud.githubusercontent.com/assets/3706122/18183691/ccf81082-7095-11e6-8680-1d94c1db6aa7.png)

To compile unit test for matrix test cases:
>g++ test/matrix_tests.cpp -I /usr/local/include/pqxx -I include/ -I src/ -o build/matrix_test -lpqxx -lpq -lsodium

To compile fuzzing test with afl:
>afl-g++ test/fuzzing_tests.cpp -I /usr/local/include/pqxx -I include/ -I src/ -o test/build/fuzzing_test -lpqxx -lpq -lsodium
>afl-fuzz -i /home/sgalvez/projects/matrixes/test -o /home/sgalvez/projects/matrixes/test/in/ /home/sgalvez/projects/matrixes/build/matrix_test -I /usr/local/include/pqxx -I include/ -I src/ -lpqxx -lpq -lsodium

