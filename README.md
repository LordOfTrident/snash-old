<p align="center">
	<h1 align="center">Snash</h2>
	<p align="center">A shell for unix/linux</p>
</p>
<p align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-GPL-blue?color=7aca00"/>
	</a>
	<a href="https://github.com/LordOfTrident/snash-old/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/snash-old?color=0088ff"/>
	</a>
	<a href="https://github.com/LordOfTrident/snash-old/pulls">
		<img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/snash-old?color=0088ff"/>
	</a>
	<br><br><br>
</p>

A [shell](https://en.wikipedia.org/wiki/Unix_shell) for [unix](https://en.wikipedia.org/wiki/Unix) and
[linux](https://en.wikipedia.org/wiki/Linux) systems. This is still a work in progress and ALOT has yet to be
implemented. Snash stands for Snail Shell.

## Features
- [X] Input
- [X] Variables list
- [X] Lexer
- [ ] Parser
- [ ] Executor
- [ ] Starting programs + passing parameters
- [ ] Changing working directory (cd)
- [ ] Setting Snash/environment variables
- [ ] If statements
- [ ] Loops

## Known bugs
- Readline not working properly with prompt escape sequences

## Make
Use `make all` to see all the make targets.

### Compiling
Run `make` or `make compile` which will create a binary in the `bin/` folder.

> If you want to change the compiler make uses, set the `C` variable. Example: `make C=gcc`

### Installing
To install the shell, run `make install` and then you can use `snash` anywhere.
