#! /bin/sh

# print a git diff of the files
git merge-file -p t1-1.org t1-0.org t1-2.org
# run the program with some test files
../src/org-merge-driver t1-1.org t1-2.org t1-0.org tout.org
cat tout.org
