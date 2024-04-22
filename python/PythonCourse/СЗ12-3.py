numerator, denominator = int(input()), int(input())


def changed_div(numerator, denominator):
    try:
        return numerator / denominator
    except ZeroDivisionError:
        return numerator


print(changed_div(numerator, denominator))
