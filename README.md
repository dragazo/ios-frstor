# ios-frstor
ios-frstor (i/o stream format restore) is a single C++ header that adds a RAII type called `basic_iosfrstor` for restoring stream formats. `basic_iosfrstor` is a template class that takes a character type and a traits type *(just like standard streams do)*, which allows it to be extremely flexible in use.

Two typedefs are added for convenience: `iosfrstor` for use with *normal* streams and `wiosfrstor` for *wide* streams.

If you want more manual control, there's also `basic_iosfmt` which holds all format settings for a stream and can be used to manually save and load said settings.

Two typedefs are added for convenience: `iosfmt` for use with *normal* streams and `wiosfmt` for *wide streams.

## Example of Use

```cpp
void log_error(std::ofstream &log_file, int line_no, int err_code, const std::string &err_msg)
{
    // create iosfrstor object to manage previous log_file format
    iosfrstor _frstor(log_file);
    
    // -- change formatting and do stuff with log_file -- //
    
    log_file << std::noshowpos << std::showbase << std::nouppercase;
    
    log_file << "line " << std::dec << std::setfill(' ') << std::setw(5) << line_no  << ": error "
                        << std::hex << std::setfill('0') << std::setw(4) << err_code << " - "
                        << err_msg << std::endl;
    
    // _frstor goes out of scope and restores old log_file format - as if we'd never changed it
}
```
