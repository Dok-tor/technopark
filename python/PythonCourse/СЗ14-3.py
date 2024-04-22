import datetime

string_datetime = input()


def parse_time(s):
    data_time = datetime.datetime.strptime(s, '%Y %m %d %H %M %S')
    delta = datetime.timedelta(days=1)
    result = data_time + delta
    return result.day


print(parse_time(string_datetime))
