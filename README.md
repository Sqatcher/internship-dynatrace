# Dynatrace Gdansk Summer Internship 2023 C++ Assignment
## The solution


### Comment regarding tests
I wasn't sure how to use Google Tests since I wasn't allowed to modify `main.cpp`, so I wrote my own shell script
in the `project/test` folder. It runs the main program with arguments taken from each line of `tests.txt` file and compares its output
(which is stored in `project/test/_output`) with the right `answer{}.txt` file. New test data has been added in `project/test/data`.

Although this approach is not suited for mass production since answers.txt files are connected to tests by the order of their invocation,
it was enough to test some basic edge cases as well as wrong JSON configuration.