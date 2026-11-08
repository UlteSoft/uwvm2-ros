#!/usr/bin/env python3
"""Build the small UWVM bridge against an existing IDE and LSP4IJ; no downloads."""
import argparse,os,subprocess,tempfile,zipfile
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--ide',required=True);p.add_argument('--lsp4ij',required=True);p.add_argument('--output',required=True);a=p.parse_args()
ide=Path(a.ide).resolve();plugin=Path(a.lsp4ij).resolve();output=Path(a.output).resolve()
root=Path(__file__).resolve().parent
classpath=list((ide/'lib').glob('*.jar'))+list((plugin/'lib').glob('*.jar'))
if not classpath or not (plugin/'lib/lsp4ij-0.21.0.jar').is_file():raise SystemExit('Select the qualified existing LSP4IJ 0.21.0 directory')
compiler=ide/'jbr/bin/javac'
output.parent.mkdir(parents=True,exist_ok=True)
with tempfile.TemporaryDirectory(prefix='uwvm-idea-build-') as temporary:
 classes=Path(temporary)
 subprocess.run([str(compiler),'-J-XX:ActiveProcessorCount=2','--release','21','-sourcepath','','-implicit:none','-classpath',os.pathsep.join(map(str,classpath)),
                 '-d',str(classes),*map(str,(root/'src').rglob('*.java'))],check=True)
 with zipfile.ZipFile(output,'w',zipfile.ZIP_DEFLATED) as archive:
  # IntelliJ needs a real jar, not loose classes under the plugin directory.
  jar=classes/'uwvm-debug.jar'
  with zipfile.ZipFile(jar,'w',zipfile.ZIP_DEFLATED) as nested:
   for path in classes.rglob('*.class'):nested.write(path,str(path.relative_to(classes)))
   nested.write(root/'plugin.xml','META-INF/plugin.xml')
  archive.write(jar,'uwvm-debug/lib/uwvm-debug.jar')
print(output)
