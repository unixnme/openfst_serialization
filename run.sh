set -e
find . -name '*.fst' | xargs -n 1 -I '{}' fstdraw --portrait=1 "{}" "{}".dot
find . -name '*.fst.dot' | xargs -n 1 -I '{}' dot -Tpdf "{}" -o "{}".pdf
