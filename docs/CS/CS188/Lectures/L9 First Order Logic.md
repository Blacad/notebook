- FOL是First-order logic的缩写，PL是Propositional logic的缩写
- Spectrum of representations 表示范畴
	- Atomic 原子 --- 将世界或问题的每个**可能状态**看作一个**单一、完整、不可再分的单元**。就像棋盘上的一个特定局面，或者迷宫中的一个特定位置。 --- 状态 A
		- Search、Game
	- Factored 因子化 --- 将原子状态**分解为一组变量/特征（Factors）**。状态由这些变量的取值共同定义。这引入了**内部结构**，允许我们描述状态的属性以及属性之间的**局部关系**。 ---- 状态{a=T,b=F,....}(往往是真假)
		- Planning、propositional logic、Bayes nets
	- Structured 结构化 --- 在因子化的基础上，进一步引入了**对象（Objects）、关系（Relations）和量词（Quantifiers）** 的概念。它不仅能描述属性，还能描述**对象之间的抽象关系**，并能表达适用于**一类对象**的通用规则。 ---- 状态 {Fighting(ObjectA,ObjectB),......}
		- First-order logic、databases、logic programs、probabilistic programs


- Expressive power 表达能力
	- First-Order logic 的表达能力远超 Propositional logic
	- 比如 基于 successor-state axiom 和 FOL我们可以写出Alive的规则$\forall t \ Alive(t) \iff [Alive(t-1) \wedge \neg \exists g,x,y \ [Ghost(g) \wedge At(Pacman,x,y,t-1)\wedge At(g,x,y,t-1)]]$
		- 如果这个需要用 propositional logic 来表示会非常复杂，我们需要为每个 t 创造这样的式子，同时后面的鬼怪和位置判断也会非常复杂都需要列出来
	- 再比如 表示 Pacman同时只能在一个位置，其中FOL表示First-order Logic，PL表示Propositional logic![](../assets/FOLEPL.png)


- ==Possible World --- 可能世界==
	- 对于 Structured 的 FOL来讲，它有三个重要的东西 Function、Relation(relation = predicate谓词)、Constant(常量通常指向某个对象)，因此可能世界就需要有上述三者的定义，并由它们描述可能世界
	- 比如 $Knows(A, BFF(B))$  其中BFF是function、Knows是Relation。那么，A,B,BFF(B)都可以表示对象，可能世界就是 它们指代的对象究竟是哪一个(BFF有明确的历程所以B确认后BFF(B)也就确认了)，也就是说==可能世界的关键在于所有的constant指向的对象到底是什么==

- ==Syntax and semantics==
	- Term 术语用于指向对象
		- 它可以是 constant ---- B=2
		- 也可以是 function with argument --- BFF(B)
	- Atomic sentence 原子句是一个基本命题
		- Relation with arguments --- Knows(A,B)
		- An equality between terms --- BFF(B) = B
	- Complex sentence
		- 利用 logic 连接起来的，即 $\wedge$ 等
		- 利用 量词Quantifiers限制， 即 $\forall \ \exists$


- ==Inference in FOL==
	- 本质上与PL一样，但是如果我们entail一个 存在量词修饰的query最好再补充一个binding使得信息更多![](../assets/FOLinference.png)
	- Propositionalization 命题化
		- 将 FOL 转换为 PL，然后通过L8中的SAT solver 去解决即 使用 DPLL 验证可满足性
	- Lifted inference 提升推理
		- 将 inference rules 直接应用于 FOL，采用==肯定前件式(Modus Ponens)==的方法去做 即 我们的KB中有 imply 那么我们结合KB的其他条件使得 imply的前半部分为True，就可以推得它的后半部分为 True
		- 比如 $KB=\forall Knows(x,Obama) \wedge \forall y,z \ Knows(y,z) \implies Likes(y,z)$
			- 我们取 $\sigma = \{y/x,z/Obama\}$ 这样前件式为T就可以得到 $Likes(x,Obama)$ 为T
		- 我们采用 Prolog (backward chaining), Datalog (forward chaining), production rule systems (forward chaining), resolution theorem provers去解决此类推理
	- 推理在一般情况下是半可决定semidecidable的；许多问题在实践中可以高效解决(半可决的意思是如果一阶逻辑（FOL）句子不可满足，将找到某个有限k的矛盾；如果不，则可能永远继续)