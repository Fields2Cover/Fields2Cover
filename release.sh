#!/usr/bin/env bash
# Cut a Fields2Cover release: bump the version in CMakeLists.txt and package.xml,
# turn the [Unreleased] changelog section into the release section, commit, push
# and publish the GitHub release vX.Y.Z -- which triggers the PyPI workflow.
# Everything here can also be done by hand; the script only keeps the steps in
# order and refuses to continue when a precondition is not met.
#
# Usage: scripts/release.sh X.Y.Z
set -euo pipefail

[ $# -eq 1 ] || { echo "usage: $0 X.Y.Z" >&2; exit 2; }
NEW="$1"
[[ "$NEW" =~ ^[0-9]+\.[0-9]+\.[0-9]+$ ]] || { echo "error: version must be X.Y.Z" >&2; exit 2; }

cd "$(git rev-parse --show-toplevel)"

# --- preconditions -----------------------------------------------------------
branch="$(git rev-parse --abbrev-ref HEAD)"
[ "$branch" = "main" ] || { echo "error: releases are cut from main (current branch: $branch)" >&2; exit 1; }
[ -z "$(git status --porcelain)" ] || { echo "error: working tree is not clean" >&2; exit 1; }
git fetch -q origin main
[ "$(git rev-parse HEAD)" = "$(git rev-parse origin/main)" ] || { echo "error: main is not in sync with origin/main" >&2; exit 1; }
command -v gh >/dev/null || { echo "error: the GitHub CLI (gh) is required" >&2; exit 1; }
gh auth status >/dev/null 2>&1 || { echo "error: run 'gh auth login' first" >&2; exit 1; }

OLD="$(sed -nE 's/^[[:space:]]*VERSION[[:space:]]+([0-9]+\.[0-9]+\.[0-9]+)[[:space:]]*$/\1/p' CMakeLists.txt)"
[ -n "$OLD" ] || { echo "error: could not read VERSION from CMakeLists.txt" >&2; exit 1; }
if [ "$OLD" = "$NEW" ] || [ "$(printf '%s\n%s\n' "$OLD" "$NEW" | sort -V | tail -n1)" != "$NEW" ]; then
  echo "error: new version $NEW must be greater than the current $OLD" >&2; exit 1
fi
if git rev-parse -q --verify "refs/tags/v$NEW" >/dev/null; then
  echo "error: tag v$NEW already exists" >&2; exit 1
fi
grep -q '^## \[Unreleased\]' CHANGELOG.md || { echo "error: CHANGELOG.md has no '## [Unreleased]' section" >&2; exit 1; }
grep -q "<version>$OLD</version>" package.xml || { echo "error: package.xml does not contain <version>$OLD</version>" >&2; exit 1; }

# --- release notes = the [Unreleased] section of the changelog ---------------
NOTES="$(awk '/^## \[Unreleased\]/{flag=1; next} /^## \[/{flag=0} flag' CHANGELOG.md)"
if [ -z "$(printf '%s' "$NOTES" | tr -d '[:space:]')" ]; then
  echo "error: the [Unreleased] section of CHANGELOG.md is empty -- nothing to release" >&2; exit 1
fi

# --- apply -------------------------------------------------------------------
TODAY="$(date +%Y-%m-%d)"
sed -i.bak -E "s/^([[:space:]]*VERSION[[:space:]]+)$OLD([[:space:]]*)$/\1$NEW\2/" CMakeLists.txt
sed -i.bak -E "s|<version>$OLD</version>|<version>$NEW</version>|" package.xml
sed -i.bak -E "s/^## \[Unreleased\]$/## [$NEW] - $TODAY/" CHANGELOG.md
rm -f CMakeLists.txt.bak package.xml.bak CHANGELOG.md.bak

git --no-pager diff
printf '\nRelease %s: commit, push to origin/main and publish the GitHub release v%s? [y/N] ' "$NEW" "$NEW"
read -r answer
if [ "$answer" != "y" ] && [ "$answer" != "Y" ]; then
  git checkout -- CMakeLists.txt package.xml CHANGELOG.md
  echo "aborted, changes reverted"; exit 1
fi

git commit -q -am "Release $NEW"
git push -q origin main
gh release create "v$NEW" --target main --title "v$NEW" --notes "$NOTES"

echo
echo "Release v$NEW published. The PyPI workflow is running at:"
echo "  $(gh repo view --json url --jq .url)/actions/workflows/pypi.yml"
echo "If the 'pypi' environment requires reviewers, approve the deployment there."
