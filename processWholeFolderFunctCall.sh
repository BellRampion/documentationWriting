#!/bin/bash
# /* COPYRIGHT AND LICENSE
# Copyright 2019 by Bailie Livingston.
# This program is free software: you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation, either version 3 of the License, or
#     (at your option) any later version.
# 
#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.
#
#     You should have received a copy of the GNU General Public License
#     along with this program.  If not, see <https://www.gnu.org/licenses/>.
# */
#**Script to run a command (or set of commands) against an entire folder's files

#NOTE: This script fails if the folder is empty. Otherwise, works great

progOutput="callsOutput.txt"
progFinalOutput="totalOutput.txt"
finalOutput="finishedOutput.txt"

#Dir path must be hard-coded without quotes
for filename in /mnt/c/Users/SSIntern/Documents/base/WebCalendarPicker/*.xojo_code; do
    java FunctionCallFinder "$filename"
    ./appendFile "$progOutput" "$finalOutput"
done
