import os
import shutil
import sys
import platform
import re
import ssh_key_masking


def lines(fn):
    with open(fn) as fp:
        return fp.readlines()


def determine_version_str():
    major = minor = gold = 0
    for line in lines('version'):
        if line.startswith('GAMS_DISTRIB'):
            rhs = line.split('=')[1].strip()
            if 'MAJOR' in line:
                major = rhs
            elif 'MINOR' in line:
                minor = rhs
            elif 'GOLD' in line:
                gold = rhs
    return f'{major}.{minor}.{gold}'


def execute(command, wei_shell='pwsh'):
    cmd = command
    if platform.system() == 'Windows':
        if wei_shell == 'pwsh':
            cmd = 'powershell -NoProfile -ExecutionPolicy ByPass ' + command
        elif wei_shell == 'bash':
            cmd = 'C:/tools/msys64/usr/bin/bash -l -c "' + cmd + '"'
    print(cmd)
    os.system(cmd)


def fetch_from_url_command(url):
    return dict(Windows=f'Invoke-WebRequest "{url}" -OutFile "{url.split("/")[-1]}" -UseBasicParsing',
                Darwin=f'curl -OL {url}',
                Linux=f'curl -OL {url}')[platform.system()]


def download_command(platf):
    gams_version = determine_version_str()
    print(f'Fetching installers for GAMS version {gams_version} from web...')
    return \
        dict(
            leg=fetch_from_url_command(f'https://d37drm4t2jghv5.cloudfront.net/distributions/{gams_version}/linux/linux_x64_64_sfx.exe'),
            wei=fetch_from_url_command(f'https://d37drm4t2jghv5.cloudfront.net/distributions/{gams_version}/windows/windows_x64_64.exe'),
            deg=fetch_from_url_command(f'https://d37drm4t2jghv5.cloudfront.net/distributions/{gams_version}/macosx/osx_x64_64_sfx.exe'))[platf]


def install():
    install_commands = {
        'Linux': [
            'unzip -q linux_x64_64_sfx.exe',
            'mv gams*_linux_x64_64_sfx gamsdist',
            'rm linux_x64_64_sfx.exe'],
        'Windows': [
            './windows_x64_64.exe /sp- /noLicense=yes /silent /noIcons /desktopIcons=no /dir=gamsdist /currentuser',
            'Start-Sleep -Seconds 60',
            'Remove-Item windows_x64_64.exe'],
        'Darwin': [
            'unzip -q osx_x64_64_sfx.exe',
            'mv gams*_osx_x64_64 gamsdist',
            'rm osx_x64_64_sfx.exe']
    }
    for command in install_commands[platform.system()]:
        execute(command)


def fetch_branch_through_ssh(branch_name, platforms, masked_ssh_key=None):
    print(f'Fetching installers for GAMS development branch {branch_name} from web.')
    keyfile = None
    if masked_ssh_key:
        print(f'Using masked ssh key of len {len(masked_ssh_key)}...')
        user_dir = dict(Linux='/root', Darwin='/Users/gitlab', Windows='C:/home/distrib')[platform.system()]
        os.makedirs(f'{user_dir}/.ssh', exist_ok=True)
        shutil.copyfile('ci/config', user_dir + '/.ssh/config')
        keyfile = user_dir + '/.ssh/porting'
        with open(keyfile, 'w') as fp:
            fp.write(ssh_key_masking.unmask_key(masked_ssh_key))
        os.chmod(keyfile, 0o600)

    wei_shell = 'bash'

    for platf in platforms:
        var_prefix = '$env:' if platform.system() == 'Windows' and wei_shell == 'pwsh' else '$'

        def civar(name): return var_prefix + name

        variant_name = ('finishednb_' if branch_name == 'master' else '_' if re.match('dist\\d+',
                                                                                      branch_name) else 'nb_') + platf
        install_filename = dict(leg='linux_x64_64_sfx', deg='osx_x64_64_sfx', wei='windows_x64_64')[platf]
        installer_path = f'{civar("PF_PATH_PREFIX")}/{branch_name}/latest{variant_name}/{install_filename}.exe'

        command = f'scp -v -oStrictHostKeyChecking=no -oPort={civar("PF_SSH_PORT")} {civar("PF_SSH_USER")}@{civar("PF_SSH_SERVER")}:{installer_path} .'
        execute(command, wei_shell)

    if masked_ssh_key:
        os.remove(keyfile)


def fetch_installers_from_web(platforms):
    for platf in platforms:
        execute(download_command(platf))


def main(args):
    if args[1].startswith('fetch'):
        platforms = ['wei', 'leg', 'deg']
        if '_' in args[1]:  # allow e.g. fetch_leg
            platforms = [args[1].split('_')[1]]
        if args[2] == '0':
            fetch_installers_from_web(platforms)
        else:
            fetch_branch_through_ssh(args[2], platforms, args[3] if len(args) >= 4 else None)
    elif args[1] == 'install':
        install()


if __name__ == '__main__':
    main(sys.argv)
