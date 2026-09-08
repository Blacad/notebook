def minimum_mewtations(typed, source, limit):
    """A diff function for autocorrect that computes the edit distance from TYPED to SOURCE.
    This function takes in a string TYPED, a string SOURCE, and a number LIMIT.

    Arguments:
        typed: a starting word
        source: a string representing a desired goal word
        limit: a number representing an upper bound on the number of edits

    >>> big_limit = 10
    >>> minimum_mewtations("cats", "scat", big_limit)       # cats -> scats -> scat
    2
    >>> minimum_mewtations("purng", "purring", big_limit)   # purng -> purrng -> purring
    2
    >>> minimum_mewtations("ckiteus", "kittens", big_limit) # ckiteus -> kiteus -> kitteus -> kittens
    3
    """
    if typed == source: # Base cases should go here, you may add more base cases as needed.
        # BEGIN
        return 0
        # END
    def helper(i, j, edits):
        # 剪枝：当前编辑次数已超过limit，停止递归
        if edits > limit:
            return limit + 1

        # 若s1和s2都处理完，返回当前编辑次数
        if i == len(typed) and j == len(source):
            return edits

        # s1处理完，剩下的只能插入
        if i == len(typed):
            return edits + (len(source) - j)

        # s2处理完，剩下的只能删除
        if j == len(source):
            return edits + (len(typed) - i)

        if typed[i] == source[j]:
            # 字符相同，不需要编辑
            return helper(i + 1, j + 1, edits)
        else:
            # 尝试三种编辑操作：
            insert = helper(i, j + 1, edits + 1)   # 插入s2[j]到s1
            delete = helper(i + 1, j, edits + 1)   # 删除s1[i]
            replace = helper(i + 1, j + 1, edits + 1)  # 将s1[i]替换为s2[j]
            return min(insert, delete, replace)

    return helper(0, 0, 0)