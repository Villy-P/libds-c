# Contributing

Thanks for your interest in contributing to `libds-c`! This document covers everything you need to get set up and submit changes.

## Setup

To set up `libds-c` locally, run:
 
```sh
./scripts/setup.sh
```
 
This will install all required packages, set up the pre-commit checks, and build the project.

> **Note:** `setup.sh` is tested on Linux and macOS. If you're on another platform, please open an issue if you run into problems.

## Workflow
 
`libds-c` uses [Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/); all commits and PR titles must follow this convention (e.g. `fix: correct off-by-one in ring buffer`).
 
If you're planning a large or breaking change, please open an issue first to discuss the approach before submitting a PR.
 
Branch names should follow `<type>/<short-description>`, e.g. `feat/add-hashmap` or `fix/leak-in-list-free`, or `<issue#>/<issue>` if you are developing an issue.

## Code Style

`libds-c` uses `clang-format` for linting. You can check formatting locally with:
 
```sh
make clang-format
```
 
This check runs automatically as a pre-commit hook (installed by `setup.sh`) and is re-verified in CI. A commit or PR will fail if formatting doesn't pass.

## Testing

Tests live in the `test` directory. Run the full test suite with:
 
```sh
make test
```
 
New functionality should include corresponding tests. Bug fixes should include a regression test where practical.

## Pull Requests

When opening a pull request:
 
- Use a title that follows Conventional Commits and accurately describes the change
- Link the issue it resolves, if applicable, using `Fixes #123` or `Closes #123`
- Keep PRs focused on one issue at a time if possible
- Ensure all CI checks pass before requesting review

## Issue Reporting

To report a bug or request a feature, open a new issue. For bug reports, please include:
 
- Your OS and compiler version
- Steps to reproduce
- Expected vs. actual behavior
