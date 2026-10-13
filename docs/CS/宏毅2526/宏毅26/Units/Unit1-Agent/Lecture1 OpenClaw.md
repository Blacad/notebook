
- OpenClaw 是指 AI Agent 中模型之外的部分，也就是 Harness
	- [图示](../../Slides/Unit1/intro.pdf#page=10)


- 有一个由 AI Agent 组成的社交媒体 [moltbook](https://moltbook.com/)

- AI Agent能力
	- 知道自己是谁
	- 工具 使用
	- skill 使用
	- 记忆
	- 定时工作
	- 长时间自主运作


- 知道自己是谁
	- 通过 System prompt 描述 AI Agent 的身份等信息，每次呼叫时都附加在用户输入上，因此 AI Agent 能够知道自己的信息
		- [图示](../../Slides/Unit1/intro.pdf#page=22)
	- System prompt 中往往包含 身份有关信息、模型行为信息、工具的元信息、技能的元信息、之前的记忆的元信息 等等等
		- [图示](../../Slides/Unit1/intro.pdf#page=23)
	- [多轮对话图示](../../Slides/Unit1/intro.pdf#page=25)

- 工具使用
	- 模型根据任务返回要使用的工具，Harness 解析模型的工具调用去实际调用工具并得到工具结果，将工具结果再给 AI，如此循环往复直到模型认为本次任务完成
		- `[tool_use]...` 
		- `[Done]`
		- [图示27-29](../../Slides/Unit1/intro.pdf#page=27)
	- AI Agent 的防御机制
		- 防止高危 工具调用 执行
		- 两种常见做法
			- 在 System prompt 中指定 工具调用的危险层级，比如有些指令不要执行
			- 在 Harness 层面进行 沙箱 隔绝 或者 某些高危指令需要人类判断是否执行
	- AI Agent 可以自创建工具
	- 特殊的工具 --- Sub-agent 
		- sessions_spawn
		- 优势
			- 可以让 主Agent 的上下文更加干净，专注于 high-level 的事情(context engineering)
			- sub-agent 可以使用专门模型，甚至可能做得比 主 Agent 好
		- [图示36-38](../../Slides/Unit1/intro.pdf#page=36)
		- sub-agent 禁用 spawn，避免无限套娃

- Skill 使用
	- Skill 就是 工作的 SOP(Standard Operating Procedure)
	- Skill 的组成 往往只包含文档，但是更复杂的技能还会包含各种 脚本
	- Skill 调用过程
		- Harness 会将 所有skill的元信息放进 System prompt 中，由 AI 按照任务需求有需查看所需 skill 的详细信息，并按照 skill 的描述执行
		- [图示44-45](../../Slides/Unit1/intro.pdf#page=44)


- 记忆系统 RAG
	- `AGENTS.md` 中会告知 AI Agent 记录历史信息的元信息，比如 Daily notes 记录在 `memory/YYYY-MM-DD.md` ，Long-term 记录在 `MEMORY.md` 
		- [图示49-50](../../Slides/Unit1/intro.pdf#page=49)
	- 跨 session 的记忆靠工具获取
		- `memory_search(MEMORY.md, memory/*.md)`
		- `memory_get(target.md)`

- 心跳(HEARTBEAT)机制
	- Harness 每隔一段时间会发送一段固定消息给 AI，让 AI 去做某些事情，比如让 AI 朝着自己定的目标前进
		- 示例：`Read HEARTBEAT.md if it exists (workspace context). Follow it strictly. Do not infer or repeat old tasks from prior chats. If nothing needs attention, reply HEARTBEAT_OK.`
		- [图示](../../Slides/Unit1/intro.pdf#page=54)
	- Cron Job
		- 给 Agent 说每天中午12:00看论文
		- Agent 执行 `[tool_use]cron Job`
			- 可以设定中午12:00看论文的定时任务
		- 设定好任务后，每到定时任务的规定时间，Harness 都会向 AI 发布该任务，让 AI 去做
		- Cron Job 还让 Agent 学会了等待
			- 比如某些工具可能需要等待非常长的时间，这个时候 Agent 自己可以使用 cron job 设置 几分钟检查一次 直到完成

- Context Compression
	- 当 session 的对话信息超出上下文窗口时，会将较旧的信息给 AI 让它生成 summary，然后使用 summary 替代较旧信息，实现 Context Compression
	- 当然还有更多压缩方法，比如 省略工具输出的细节