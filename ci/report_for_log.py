import sys


def is_warning(line):
    return ' warning:' in line


def escape_xml(line):
    return line.replace('<', 'lt').replace('>', 'gt').replace('"', '\'').replace('&', 'and').strip()


def lines_starting_with_space(lines, start_ix):
    res = []
    while start_ix < len(lines):
        first_char = lines[start_ix][0]
        if first_char != ' ' and first_char != '\t':
            break
        res += lines[start_ix]
        start_ix += 1
    return res


def extract_warnings(fn):
    with open(fn) as fp:
        lines = fp.readlines()
    print(f'Extracting warnings from file {fn} with {len(lines)} lines...')
    return [escape_xml(line + ''.join(lines_starting_with_space(lines, ix + 1))) for ix, line in enumerate(lines) if
            is_warning(line)]


def main(args):
    warnings = extract_warnings(args[2])
    compiler_name = args[1]
    ostr = '<?xml version="1.0" encoding="UTF-8"?>\n<testsuites>\n'
    ostr += f'<testsuite name="{compiler_name}-warnings" errors="{len(warnings)}" tests="{len(warnings)}">\n'
    for ix, warning in enumerate(warnings):
        parts = warning.split(":")
        location = ':'.join(parts[0:2])
        message = ':'.join(parts[3:]).strip()
        ostr += f'<testcase id="{ix + 1}" name="{location}"><failure message="{message}" type="error" /></testcase>\n'
    ostr += "</testsuite>\n</testsuites>\n"
    print(ostr)
    with open(args[3], 'w') as fp:
        fp.write(ostr)


if __name__ == '__main__':
    main(sys.argv)
