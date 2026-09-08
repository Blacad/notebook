```
def printed(f):
    """Print the result of f(args) and return it.
    >>> printed(pow)(2, 8)
    Result: 256
    256
    >>> printed(abs)(-10)
    Result: 10
    10
    """
    def print_and_return(*args):
        result = f(*args)
        print('Result:', result)
        return result
    return print_and_return
```