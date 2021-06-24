# showballoon
showballoon is 32bit windows executable that shows a balloon (or tooltip).

## Command Line
If launched without command line options, it show a help message.

## Actual use
```
showballoon.exe /title:"My Title" /icon:"C:\Linkout\CommonExe\showballoon.exe" /defaulticon /duration:5000 /balloonicon:1 "This is a balloon message!"
```

## To pass non-ascii characters
You need to utf8-urlencode the non-ascii characters.
```
showballoon.exe /title:"%E7%A7%81%E3%81%AE%E3%82%BF%E3%82%A4%E3%83%88%E3%83%AB" /icon:"C:\Linkout\CommonExe\showballoon.exe" /defaulticon /duration:5000 /balloonicon:1 %E3%81%93%E3%82%8C%E3%81%AF%E3%83%90%E3%83%AB%E3%83%BC%E3%83%B3%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8%E3%81%A7%E3%81%99%EF%BC%81
```

