# 🍦 ssws - Super simple web server

> [!CAUTION]
> Under development as pre-alpha (currently not working)

A minimal static file server.

## Usage

```sh
ssws --dir my-site/ --host 0.0.0.0 --port 8000
```

## Help

```sh
🍦 Super simple web server

Usage: ssws [OPTIONS]

Options:
  -d, --dir   <PATH>	Root directory	[default: .]
  -a, --host  <ADDR>	Host address	[default: 127.0.0.1]
  -p, --port  <PORT>	Host port       [default: 80]
      --index <PATH>	Root HTML file	[default: index.html]
      --404   <PATH>	404 HTML file	[default: 404.html]
      --50x   <PATH>	50x HTML file	[default: 50x.html]
  -v, --verbose		    Increase logging verbosity
  -q, --quiet		    Decrease logging verbosity
  -h, --help		    Print this message
```
