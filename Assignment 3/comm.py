#!/usr/bin/python

import sys
from optparse import OptionParser

class comm:
    def __init__(self, lines1, lines2):
        self.lines1 = lines1
        self.lines2 = lines2
    
    def unique1(self):
        self.unique_lines1 = []
        for index1, line1 in enumerate(self.lines1):
            for index2, line2 in enumerate(self.lines2):
                is_unique = True
                if line1 == line2 and index1 == index2:
                    is_unique = False
                    break

            if is_unique == True:
                self.unique_lines1.append([line1, index1, "unique1"])
        
        return self.unique_lines1

    def unique2(self):
        self.unique_lines2 = []
        for index2, line2 in enumerate(self.lines2):
            for index1, line1 in enumerate(self.lines1):
                is_unique = True
                if line2 == line1 and index2 == index1:
                    is_unique = False
                    break

            if is_unique == True:
                self.unique_lines2.append([line2, index2, "unique2"])

        return self.unique_lines2

    def common(self):
        self.common_lines = []
        for index1, line1 in enumerate(self.lines1):
            for index2, line2 in enumerate(self.lines2):
                if line1 == line2 and index1 == index2:
                    self.common_lines.append([line1, index1, "common"])
                    break

        return self.common_lines

    def col12(self):
        arr1 = self.unique1()
        arr2 = self.unique2()
        temp = arr1 + arr2

        for line in temp:
            if line[2] == "unique2":
                line[0] = "\t" + line[0]

        temp = sorted(temp, key=lambda line: (line[1], line[0].strip()[0]))
        return temp
    
    def col13(self):
        arr1 = self.unique1()
        arr_common = self.common()

        temp = arr1 + arr_common
        for line in temp:
            if line[2] == "common":
                line[0] = "\t" + line[0]

        temp = sorted(temp, key=lambda line: (line[1], line[0].strip()[0]))
        return temp

    def col23(self):
        arr2 = self.unique2()
        arr_common = self.common()

        temp = arr2 + arr_common
        for line in temp:
            if line[2] == "common":
                line[0] = "\t" + line[0]

        temp = sorted(temp, key=lambda line: (line[1], line[0].strip()[0]))
        return temp

    def all(self):
        arr1 = self.unique1()
        arr2 = self.unique2()
        arr_common = self.common()

        arr_all = arr1 + arr2 + arr_common
        for line in arr_all:
            if line[2] == "unique2":
                line[0] = "\t" + line[0]
            elif line[2] == "common":
                line[0] = "\t\t" + line[0]

        arr_all = sorted(arr_all, key=lambda line: (line[1], line[0].strip()[0]))
        
        return arr_all

    def all_u(self, suppress1, suppress2, suppress3):
        arr1 = []
        arr2 = []
        for line in self.lines1:
            arr1.append(line)
        for line in self.lines2:
            arr2.append(line)
        
        arr_common = []
        for line1 in arr1:
            for line2 in arr2:
                if line1 == line2:
                    arr_common.append(line1)
                    arr2.remove(line2)

        for line2 in arr_common:
            for line1 in arr1:
                if line2 == line1:
                    arr1.remove(line1)
                    break

        arr_all_u = []
        
        if suppress1 == 0:
            for index, line in enumerate(arr1):
                arr_all_u.append([line, index, "unique1"])

        if suppress3 == 0:
            if suppress1 == 1 and suppress2 == 1:
                for index, line in enumerate(arr_common):
                    arr_all_u.append([line, index, "common"])
            elif suppress1 == 0 and suppress2 == 0:
                for index, line in enumerate(arr_common):
                    arr_all_u.append(["\t\t" + line, index, "common"])
            else:
                for index, line in enumerate(arr_common):
                    arr_all_u.append(["\t" + line, index, "common"])

        if suppress2 == 0:
            if suppress1 == 1:
                for index, line in enumerate(arr2):
                    arr_all_u.append([line, index, "unique2"])
            else:
                for index, line in enumerate(arr2):
                    arr_all_u.append(["\t" + line, index, "unique2"])
                    
        return arr_all_u

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]...FILE1 FILE2
Compares two files line by line and outputs the unique and common lines """    
    
    parser = OptionParser(version=version_msg,
                          usage=usage_msg)
    parser.add_option("-1", "--suppress1",
                      action="store_true", dest="suppress1", default=False,
                      help="don't output lines unique to file1")
    parser.add_option("-2", "--suppress2",
                      action="store_true", dest="suppress2", default=False,
                      help="don't output lines unique to file2")
    parser.add_option("-3", "--suppress3",
                      action="store_true", dest="suppress3", default=False,
                      help="don't output lines common to both files")
    parser.add_option("-u", "--unsorted",
                      action="store_true", dest="unsorted", default=False,
                      help="handle unsorted files")

    options, args = parser.parse_args(sys.argv[1:])

    try:
        suppress1 = int(options.suppress1)
        suppress2 = int(options.suppress2)
        suppress3 = int(options.suppress3)
        unsorted = int(options.unsorted)
    except:
        parser.error("error".
                     format(options))
    if len(args) != 2:
        parser.error("wrong number of operands")
        
    input_file1 = args[0]
    input_file2 = args[1]
    input_lines1 = []
    input_lines2 = []

    if input_file1 == "-":
        input_lines1 = sys.stdin.readlines()
    else:
        try:
            f1 = open(input_file1, 'r')
            input_lines1 = f1.readlines()
            f1.close() 
        except IOError as err:
            parser.error("I/O error({0}): {1}".
                     format(err.errno, err.strerror))
    
    if input_file2 == "-":
        input_lines2 = sys.stdin.readlines()
    else:
        try:
            f2 = open(input_file2, 'r')
            input_lines2 = f2.readlines()
            f2.close() 
        except IOError as err:
            parser.error("I/O error({0}): {1}".
                     format(err.errno, err.strerror))
       
    generator = comm(input_lines1, input_lines2)
    data = generator.all()
    if options.suppress1 == True and options.suppress2 == True and options.suppress3 == True:
        sys.stdout.write("")
    else:
        # Comm -12 / -21
        if options.suppress1 == True and options.suppress2 == True and options.suppress3 == False:
            if options.unsorted == True:
                lines = generator.all_u(1,1,0)
            else:
                lines = generator.common()
                
            for line in lines:
                sys.stdout.write(line[0])
        # Comm -13 / -31
        elif options.suppress1 == True and options.suppress2 == False and options.suppress3 == True:
            if options.unsorted == True:
                lines = generator.all_u(1,0,1)
            else:
                lines = generator.unique2()
                
            for line in lines:
                sys.stdout.write(line[0])
        # Comm -23 / -32      
        elif options.suppress1 == False and options.suppress2 == True and options.suppress3 == True:
            if options.unsorted == True:
                lines = generator.all_u(0,1,1)
            else:
                lines = generator.unique1()
                
            for line in lines:
                sys.stdout.write(line[0])
        # Comm -1
        elif options.suppress1 == True and options.suppress2 == False and options.suppress3 == False:
            if options.unsorted == True:
                lines = generator.all_u(1,0,0)
            else:
                lines = generator.col23()    
                
            for line in lines:
                sys.stdout.write(line[0])
        # Comm -2
        elif options.suppress1 == False and options.suppress2 == True and options.suppress3 == False:
            if options.unsorted == True:
                lines = generator.all_u(0,1,0)
            else:
                lines = generator.col13()    
                
            for line in lines:
                sys.stdout.write(line[0])
        # Comm -3
        elif options.suppress1 == False and options.suppress2 == False and options.suppress3 == True:
            if options.unsorted == True:
                lines = generator.all_u(0,0,1)
            else:
                lines = generator.col12()    
                
            for line in lines:
                sys.stdout.write(line[0])
        # Comm with no suppress
        elif options.suppress1 == False and options.suppress2 == False and options.suppress3 == False:
            if options.unsorted == True:
                lines = generator.all_u(0,0,0)
            else:
                lines = generator.all()
                
            for line in lines:
                sys.stdout.write(line[0])

if __name__ == "__main__":
    main()
