import os

text = input()


def write_and_read(text):
    file_path = os.path.join(os.path.abspath('/tmp'), 'my_file')
    with open(file_path, 'w') as f:
        f.write(text)
    with open(file_path, 'r') as f:
        out_text = f.read()
    return out_text


print(write_and_read(text))
