Releasing
=========

Python package on PyPI
----------------------

One-time setup (project owner):

1. Create the ``fields2cover`` project on https://pypi.org and https://test.pypi.org
   (first upload creates it; alternatively register a *pending publisher*).
2. On both, add a **Trusted Publisher**: owner ``Fields2Cover``,
   repository ``Fields2Cover``, workflow ``pypi.yml``, environment ``pypi``
   (resp. ``testpypi``).
3. In the GitHub repository create the environments ``pypi`` and ``testpypi``
   (Settings → Environments). Optionally require reviewers for ``pypi``.

Per release:

1. Bump ``VERSION`` in ``CMakeLists.txt`` (the single source of truth) and
   update ``CHANGELOG.md``.
2. Optional dry run: *Actions → PyPI → Run workflow → target: testpypi*, then
   ``pip install --index-url https://test.pypi.org/simple/ fields2cover``.
3. Publish a GitHub release with tag ``vX.Y.Z``. The ``PyPI`` workflow builds
   the sdist, installs it in a clean container, and uploads it. The workflow
   refuses to publish when the tag (``vX.Y.Z``) does not match ``VERSION``
   in ``CMakeLists.txt``.
4. A published PyPI release cannot be replaced or re-uploaded. If a release
   turns out to be broken, bump the patch version in ``CMakeLists.txt`` and
   publish a new release.
