```
def make_test_dice(*outcomes):
    index = len(outcomes) - 1  # 外部函数的变量
    def dice():
        nonlocal index  # 声明为闭包变量,即外部函数变量
        index = (index + 1) % len(outcomes)  # 修改外部变量
        return outcomes[index]
    return dice  # 返回内部函数（携带外部变量的状态）

dice = make_test_dice(1, 2, 3, 4)
print(dice(),dice(),dice(),dice(),dice())
```