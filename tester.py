import subprocess
import shutil
import os

def run_code(optimization_level, full_file_path):

    TEMP_PATH = './code/tester'

    shutil.copyfile(full_file_path, TEMP_PATH + '.txt')

    subprocess.run(['./code/project4', '-p' + str(optimization_level), './code/tester'])

    with open(TEMP_PATH + '-IC.txt', 'r') as f:
        content = f.read()

    return content



def handle_input(path, i, optimization_level):

    full_file_path = path + str(i)

    code_output = run_code(optimization_level, full_file_path + '.txt')
    
    with open(full_file_path + '-IC.txt', 'r') as f:
        correct_output = f.read()


    if correct_output != code_output:
        print(f'\nWrong case! -> {full_file_path}')
        print('Correct output is below')
        print(correct_output)
        print('Your output is below')
        print(code_output)
        print('- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n')
        return False 
    else :
        print(f'\nSuccess -> {full_file_path}')
        print('- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n')
        return True


def handle_folder(optimization_level):

    PATH = f'./samples/p{optimization_level}/sample0'

    numb_of_wrongs = 0
    numb_of_success = 0

    for i in range(1,6):
        if handle_input(PATH, i, optimization_level):
            numb_of_success += 1
        else:
            numb_of_wrongs += 1

    return (numb_of_success, numb_of_wrongs)


def main():

    numb_of_wrongs = 0
    numb_of_success = 0

    # for i in range(16):
    #     s,w = handle_folder(i)

    #     numb_of_wrongs += w
    #     numb_of_success += s

    s,w = handle_folder(0)
    numb_of_wrongs += w
    numb_of_success += s

    os.remove("./code/tester.txt")
    os.remove("./code/tester.json")
    os.remove("./code/tester-IC.txt")

    print(f'wrong cases: {numb_of_wrongs}')
    print(f'correct cases: {numb_of_success}')


if __name__ == '__main__':
    main()