# Releasing

## Python package on PyPI

One-time setup (project owner):

1. Create the `fields2cover` project on https://pypi.org and https://test.pypi.org
   (first upload creates it; alternatively register a *pending publisher*).
2. On both, add a **Trusted Publisher**: owner `Fields2Cover`, repository
   `Fields2Cover`, workflow `pypi.yml`, environment `pypi` (resp. `testpypi`).
3. In the GitHub repository create the environments `pypi` and `testpypi`
   (Settings → Environments). Optionally require reviewers for `pypi`.

Per release:

1. Make sure `CHANGELOG.md` has an `## [Unreleased]` section describing the
   release; it becomes the release notes.
2. Optional dry run: *Actions → PyPI → Run workflow → target: testpypi*, then
   `pip install --index-url https://test.pypi.org/simple/ fields2cover`.
3. From an up-to-date, clean `main` run `./release.sh X.Y.Z` (needs the
   GitHub CLI, `gh auth login`). It bumps the version in `CMakeLists.txt` and
   `package.xml`, turns `[Unreleased]` into the `[X.Y.Z]` section, shows the
   diff and — after confirmation — commits, pushes and publishes the GitHub
   release `vX.Y.Z`. The same steps can be done by hand; publishing the
   release is what triggers the upload.
4. The `PyPI` workflow builds the sdist, installs it in a clean container and
   uploads it. If the `pypi` environment requires reviewers, approve the
   deployment in the running workflow. The workflow refuses to publish when
   `package.xml` and `CMakeLists.txt` disagree or when the tag does not match
   `VERSION` in `CMakeLists.txt`.
5. A published PyPI release cannot be replaced or re-uploaded. If a release
   turns out to be broken, release a patch version the same way.
