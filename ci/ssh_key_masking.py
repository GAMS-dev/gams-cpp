import sys
import binascii


def slurp(fn):
    with open(fn) as fp:
        return fp.read()


def mask_key(fn):
    print(binascii.hexlify(slurp(fn).encode()).decode())


def unmask_key(masked_key):
    return binascii.unhexlify(masked_key.encode()).decode()


def main(args):
    if args[1] == 'mask':
        mask_key(args[2])
    elif args[1] == 'unmask':
        print(unmask_key(args[2]))


if __name__ == '__main__':
    main(sys.argv)
