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

```
showballoon.exe /title:"%E6%88%91%E7%9A%84%E9%A2%98%E7%9B%AE" /icon:"C:\Linkout\CommonExe\showballoon.exe" /defaulticon /duration:5000 /balloonicon:1 %E8%BF%99%E6%98%AF%E4%B8%AA%E6%B0%94%E7%90%83%E4%BF%A1%E6%81%AF!
```

