def repeat_deco(n=1):
    def my_decorator(func):
        def wrapper(*args, **kwargs):
            result = None
            for i in range(n):
                result = func(*args, **kwargs)
            return result
        return wrapper
    return my_decorator


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
