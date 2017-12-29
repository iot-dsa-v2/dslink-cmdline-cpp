# cmd-dslink-cpp
It is a dslink that you can control it from command line with commands.
## command: ls | list | dir
List to given directory and close the stream after listing.

`> ls` -> lists current directory

`> ls .` -> lists current directory

`> ls TARGET_PATH` -> list to target path

> If you put "stream" in front it like `stream list PATH` or `stream list` it doesn't close the stream so you can take updates in real time from server.
## command: cd
Changes current directory to given directory. If there is no directory given it goes root.

`> cd` -> navigates to the root

`> cd TARGET_PATH` -> navigates to target path

`> cd ..` -> navigates to one level back

`> cd .` -> does nothing, you are still same directory
## command: subscribe | subs
Subcribes to the path

`>subscribe PATH` -> subscribe to the path
> If you put "stream" in front it like `stream subs PATH` it doesn't close the stream so you can take updates in real time from server.
## Var
A type which based on msgpack. It can be:
* any type of json string `{"key":5}`
* integer `5`
* double `5.5`
* string `"hello"`
* `null`
* `true` | `false`
* array `[5,3,2,"sdf",3.2,null]`
* binary with BASE64 encapsulated with `"\u0001Bbytes:`BASE64`"`
## command: set
set a path with given value and optional attribe from server.

`> set PATH Var` -> set PATH with given VAR

`> set PATH Var ATTRIBUTE` -> set ATTRIBUTE in PATH with given VAR
## command: invoke
Invoke a path from server.

`> invoke PATH` -> invoke PATH

`> invoke PATH Var` -> invoke PATH with given Var
## prefix: debug
When you put in front of any command, system opens logs for dslink for command runtime.

`> debug invoke PATH`

`> debug ls`
## command: quit | q | exit
Stops its link and exits.
## command: file
Save file path from local machine for later use in Var.

`> file PATH bin PLACEHOLDERNAME` -> stores PATH as named PLACEHOLDER with noted as bin. It should be called if you want to send data as binary

`> file PATH txt PLACEHOLDERNAME` -> stores PATH as named PLACEHOLDER with noted as txt. It should be called if you want to send data as string.
> You can use these while you are giving a Var to a command. For example if we have json tex file we can load it like `> file my.json txt myjson` so you can invoke like `> invoke sys/stop 'myjson'`. It reads file content in txt and puts there. Placeholders in commands should be encapsulated with `''` You can write nested Var like `> invoke sys/stop {"key":'myjson'}`. Binary is the same : `> file lena.jpg bin my_image`
## command: filelist
Lists all defined placeholders with their filepaths
## command: timeout
Sometimes server don't respond so cmd waits TIMEOUT milisecond and than inform the user about it and gives a chance to cancel it. You can set the time between begining command and asking user to cancel it.

`> timeout 2000` -> sets 2 seconds

## Examples

`> file myzip.zip bin myzip`

`> invoke install_from_zip {"name":"mynewdslink", "zip":'myzip'}`