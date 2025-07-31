# pipex

**Program should be executed as follows:**
> ./pipex file1 cmd1 cmd2 file2

**It must take 4 arguments:**
- file1 and file2 are file names.
- cmd1 and cmd2 are shell commands with their parameters.

**It must behave exactly like the following shell command:**
> $> < file1 cmd1 | cmd2 > file2

**Bonus points if the program:**
- Handle multiple pipes.
  - This:
  > $> ./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2

  - Should behave like:
  > < file1 cmd1 | cmd2 | cmd3 ... | cmdn > file2

- Support « and » when the first parameter is "here_doc".
  - This:
  > $> ./pipex here_doc LIMITER cmd cmd1 file

  - Should behave like:
  > cmd << LIMITER | cmd1 >> file

---

### Resource

[libft](github.comKrunchiZ/libft)
