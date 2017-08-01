# Contributing to the GAMS C++ API

We highly appreciate contributions to the gams-cpp project! To submit your improvements please use GitHub to collaborate.

# How we work on gams-cpp

We recommend that you use the Qt Creator to work on gams-cpp. Below you find some information on how we
setup up the Qt Creator and how we style our code.

## Text Editor

### Behavior

  - Tabs And Indentation
    * Tab policy: spaces only
    * Tab size: 4
    * Indent size: 4
    * Align continuation lines: with spaces
  - Typing
    * ☑ Enable automatic indentation
    * Backspace indentation: None
    * Tab key performs auto-indent: In Leading White Space
  - Cleanups Upon Saving
    * (all ☑)
  - File Encodings
    * Default encoding: UTF-8
    * UTF-8 BOM: Always delete
  - Mouse and Keyboard
    * (all ☑)
    * Show help tooltipps using the mouse: On Mouseover

### Display

  - Text Wrapping
    * ☐ Enable text wrapping
    * ☑ Display right margin at column: 120
  - Display (part)
    * ☑ Display line numbers
    * ☑ Mark text changes
    * ☐ Highlight blocks
    * ☑ Animate matching parantheses
    * ☑ Highlight matching parantheses
    
## C++ style guide

We apply some common C++ style rules.

|| Rule | Example |
|---|---|---|
|1| All names are written in CamelCase style, avoid underscores unless it's common for that specific name. | myMethodName(), c_str() |
|2| Class names start uppercase | class MyClass {} |
|3| Methods and local variables start lowercase | val.isValid() |
|4| Members start with a small m | val.mValue |

Additionaly we use some rules which are quite common in Qt projects.

| | Rule | Example |
|---|---|---|
|1| Split lines that extend about 120 characters. QtCreator can be setup to mark 120 character mark.  |  |
|2| When splitting (e.g. extend 120 characters) take the separator to the next line (simplifies detection of continue-lines) |  if (this == that <br> && allTheBest) <br> <br> func(par1, para2 <br> , par3, par4) |
|3| Separate opening curly bracket for starting namespace, class and method implementation  | class MyClass() <br> {` |
|4| Trailing opening curly bracket for implement part (loops, conditional branch, ...) | for (int i: list) { |
|5| **else** and **else if** are combined in one line with curly brackets | if (something) { <br> } else if (another thing) { <br> } else { <br> }  |
|6| Closing curly brackets always are on a seperate line | if (something) { <br> } |
