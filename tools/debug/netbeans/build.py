#!/usr/bin/env python3
"""Build a small NetBeans module from an existing IDE and JDK, without downloads."""
import argparse
from pathlib import Path
import os
import subprocess
import tempfile
import zipfile

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--netbeans',required=True,type=Path)
    p.add_argument('--jdk',required=True,type=Path)
    p.add_argument('--output',required=True,type=Path)
    a=p.parse_args();here=Path(__file__).resolve().parent
    jars=sorted(a.netbeans.glob('platform/**/*.jar'))+sorted(a.netbeans.glob('ide/**/*.jar'))
    if not (a.netbeans/'ide/modules/org-netbeans-modules-lsp-client.jar').is_file():
        raise ValueError('Existing NetBeans DAP client is required')
    javac=a.jdk/'bin/javac'
    if not javac.is_file():raise ValueError('Select an existing JDK with javac')
    a.output.parent.mkdir(parents=True,exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='uwvm-netbeans-') as temporary:
        classes=Path(temporary)/'classes';classes.mkdir()
        subprocess.run([str(javac),'--release','17','-sourcepath','','-implicit:none',
            '-processor','org.netbeans.modules.openide.awt.ActionProcessor','-classpath',os.pathsep.join(map(str,jars)),'-d',str(classes),
            *map(str,sorted((here/'src').rglob('*.java')))],check=True)
        with zipfile.ZipFile(a.output,'w',zipfile.ZIP_DEFLATED) as archive:
            # JAR manifest continuation is required beyond 72 bytes.
            lines=[]
            for line in (here/'manifest.mf').read_text().splitlines():
                raw=line.encode()
                while len(raw)>70:
                    lines.append(raw[:70]);raw=b' '+raw[70:]
                lines.append(raw)
            archive.writestr('META-INF/MANIFEST.MF',b'\r\n'.join(lines)+b'\r\n')
            for item in sorted(classes.rglob('*')):
                if item.is_file():archive.write(item,str(item.relative_to(classes)))
    print(a.output.resolve())
if __name__=='__main__':main()
