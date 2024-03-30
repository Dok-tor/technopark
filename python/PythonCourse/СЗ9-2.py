def cache_deco(func):
    d = dict()

    def wrapper(args):
        if d.get(args, None) is None:
            d[args] = func(args)
            return d[args]
        return d[args]
    return wrapper


code = []
while data := input():
    code.append(data)
code = "\n".join(code)
exec(code)
