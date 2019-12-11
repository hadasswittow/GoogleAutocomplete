import os
import re
from collections import defaultdict
import json
file_data = dict()
data_dict = defaultdict(list)


def offline(directory):
    for filename in os.listdir(directory):
        if filename.endswith(".txt"):
            source_text = filename[:len(filename)-4]
            with open(os.path.join(directory, filename)) as file :
                for i, line in enumerate(file.readlines()):
                    file_data[(source_text, i)] = line
                    add_line_combinations_to_datadict(source_text, line, i)
            with open('data.json', 'w', encoding='utf-8') as f:
                json.dump(data_dict, f, ensure_ascii=False, indent=4)
            


def add_line_combinations_to_datadict(source_text, completed_sentence, offset):
    combinations = get_all_combinations(completed_sentence)
    add_combinations_to_datadict(combinations, source_text, offset)


def add_combinations_to_datadict(combinations, source_text, offset):
    for i, key in enumerate(combinations):
        add_data_to_datadict(key, source_text, offset)


def get_sentences_list(data_list):
    lst = []
    for line in data_list:
        source, offset = line.split()
        lst.append(file_data.get((source, int(offset))))
    return lst


def add_data_to_datadict(key, source_text, offset):
    data_list = data_dict[key]
    sentence = file_data.get((source_text, offset))
    if len(data_list) == 5:
        lst = get_sentences_list(data_list)
        lst.append(sentence)
        sorted_lst = sorted(lst)
        if sorted_lst[len(sorted_lst)-1] != sentence: # if we need to put the data in the dict
            index = lst.index(sorted_lst[len(sorted_lst)-1])
            del(data_list[index])
            data_list.append(f"{source_text} {offset}")
            data_dict[key] = data_list
    else:
        data_dict[key].append(f"{source_text} {offset}")


def get_all_combinations(completed_sentence):
    # gets a sentence and returns all the combinations of the sentence in windows of 1 to 10 (or sentence length)
    combinations = set()
    completed_sentence = re.sub(r'\W+\s*', ' ', completed_sentence).lower()
    rng = min(10, len(completed_sentence))
    for i in range(rng):
        for j in range(0, len(completed_sentence) - i + 1):
            word = completed_sentence[j:(j + i)]
            if word:
                combinations.add(word)

    return list(combinations)



offline(r'C:\Users\USER\PycharmProjects\GoogleProjectPartOne')
