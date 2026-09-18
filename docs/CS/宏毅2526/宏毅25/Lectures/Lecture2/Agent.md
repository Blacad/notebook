
## 基础

AI Agent
- Observation - Action
- Reward
- [图示](../../Slides/Lecture2/ai_agent.pdf#page=5)

LLM 用于 AI Agent
- goal -> obs 1 -> action 1 -> ... -> answer
- [图示](../../Slides/Lecture2/ai_agent.pdf#page=10)


LLM 用于 Agent 的优势
- 使用更多的工具，不仅仅只能下棋 像 AlphaGO 那样
- 不需要明确的 Reward，可以返回 Log 信息，让模型知道错误

AI Agent 例子
- [AI 村庄](https://arxiv.org/abs/2304.03442)
- Computer Use - GUI Agent
- 用 AI 训练 模型
- AI 做科学研究


更真实的互动情景
- 目前的互动大多都是 *回合制*
- 真实的互动应该是 *即时互动*
- [图示24-25](../../Slides/Lecture2/ai_agent.pdf#page=24-25)
 - 我感觉这里的互动定义其实有点模糊，其实就是 选择 不管新obs 还是 接受被打断


## 利用经验

根据即时的经验进行答案修正，其实就是 [上下文能力](../../Slides/Lecture2/ai_agent.pdf#page=30)

只依据上下文可能造成 超忆症，上下文信息过于冗杂，因此需要 Agent Memory

通过 Agent Memory 进行 Retrieval 只提取尽量有关的 Memory 进入上下文，其实就是 [RAG](../../Slides/Lecture2/ai_agent.pdf#page=32)
- StreamBench 是相关的研究，洞见 Retrieval 有效 + Correct 的信息更加重要

Agent Memory 的 三个重要组件
- Read - RAG
- Write
- Reflection(Knowledge Graph)
- [图示40-41](../../Slides/Lecture2/ai_agent.pdf#page=40-41)


## 利用工具

常用的工具
- Search Engine
- Python
- Other AI

Agent 使用工具，可以看作 调用 Function，因此也可以称为 Function Call

工具使用方式 System Prompt
- `<tool></tool>` 和 `<output></output>`
- [图示](../../Slides/Lecture2/ai_agent.pdf#page=50)
- 后续可能需要通过 正则匹配 等方式提取 Function Call 内容并进行Function Call 的执行拿到Obs，再返回给 Agent


众多工具与自己造工具
- 众多工具 -> 根据 任务要求 去搜索相关工具 (RAG)
- 打造工具
- [图示](../../Slides/Lecture2/ai_agent.pdf#page=57)

Agent 可能过于信赖工具
- Agent 的信息判断力可能不足，导致 Agent 过于相信工具返回的错误结果，而造成结果错误
- 其实，现在的 Agent 有一定程度的 信息判断能力，这其实体现在 Internal Knowledge 和 External Knowledge 的 [拉扯](../../Slides/Lecture2/ai_agent.pdf#page=63)

什么外部信息更容易说服 AI
- 更贴近 模型内部认知的信息 模型对其的判断能力越弱 ->  ==相比人类，Agent更相信自己的同类===
- 数据的 Meta Data 也会影响 模型对其的判断，模型更倾向于相信 更近的数据
	- [实验](../../Slides/Lecture2/ai_agent.pdf#page=66) - 两篇文章只是对换 Meta Data 的日期，Agent 的判断就不一样
	- 而 来源metadata 似乎并不是很影响

即使 工具可靠，但是 Agent 依然可能不可靠

工具与模型能力之间亦有平衡，某些场景下使用工具反而没有效率

## 做计划

Plan-and-Solve
- obs 1 -> plan 1 -> action 1 -> obs 2 -> plan 2 -> action 2 -> ....
- [图示](../../Slides/Lecture2/ai_agent.pdf#page=73)

PlanBench 考验模型做计划的能力
ARC-AGI-Series 考验模型玩电子游戏的能力


Tree Search for Agent
- 利用 动作搜索树 来做规划
- [图示89-90](../../Slides/Lecture2/ai_agent.pdf#page=89-90)
- 有些动作无法回溯 -> 与现实世界互动可能造成危害 -> World Model 脑内剧场
- World Model 脑内剧场论文
	- Is Your LLM Secretly a World Model of the Internet? Model-Based Planning for Web Agents https://arxiv.org/abs/2411.06559
- [Agent + 脑内剧场](../../Slides/Lecture2/ai_agent.pdf#page=94)
	- 脑内剧场 与 Reasoning 本质一致

总体来讲，*Plan-and-Solve 与 Reasoning 其实本质是一致的*

Reasoning 太多可能也有坏处
- The Danger of Overthinking: Examining the Reasoning-Action Dilemma in Agentic Tasks  https://arxiv.org/abs/2502.08235







