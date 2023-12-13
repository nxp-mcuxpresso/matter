# Copyright (c) 2023 Project CHIP Authors
# Copyright (c) 2023 NXP
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import subprocess
import argparse


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("-d", "--directory",
                        help="The directory from where the script was called",
                        required=True)
    parser.add_argument("-ot", "--openthread_commit",
                        help="The openthread commit to checkout to",
                        required=True)
    parser.add_argument("-ot-nxp", "--ot_nxp_commit",
                        help="The ot-nxp commit to checkout to",
                        required=True)

    return parser.parse_args()


def checkout(repo_path, commit_id):
    # Possible git errors
    reference_err = "fatal: reference is not a tree"
    unshallow_dir = "fatal: --unshallow on a complete repository does not make sense"

    git_fetch_cmd = ['git', 'fetch', '--unshallow']

    # Cd to repo
    os.chdir(repo_path)

    # Checkout to commit
    stderr = subprocess.run(['git', 'checkout', commit_id], capture_output=True).stderr.decode("utf-8")

    # Treat possible errors and retry
    if reference_err in stderr:
        stderr = subprocess.run(git_fetch_cmd, capture_output=True).stderr.decode("utf-8")
        if unshallow_dir in stderr:
            el = git_fetch_cmd.pop()
            cmd_out = subprocess.run(git_fetch_cmd)
        subprocess.run(['git', 'checkout', commit_id])

    # Cd to prev dir
    os.chdir("../" * (repo_path.count("/") + 1))


def main(args):
    # Compute paths
    path = args.directory

    if "examples" in args.directory:
        path = path + "/third_party/connectedhomeip"

    path = path + "/third_party"

    os.chdir(path)

    # Checkout ot submodules
    checkout('openthread/repo', args.openthread_commit)
    # checkout('openthread/ot-nxp', args.ot_nxp_commit)


if __name__ == '__main__':
    main(parse_args())