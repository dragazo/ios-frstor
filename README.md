# ios-frstor
ios-frstor (i/o stream format restore) is a single C++ header that adds a RAII type called `basic_iosfrstor` for restoring stream formats based on a unique contract. `basic_iosfrstor` is a template class that takes a character type and a traits type *(just like standard streams do)*, which allows it to be extremely flexible in use.

Two typedefs are added for convenience: `iosfrstor` for use with *normal* streams and `wiosfrstor` for *wide* streams.

An instance of `basic_iosfrstor` can be explicitly constructed from anything that inherits from `std::basic_ios` *(which includes all standard stream types)*, at which point it creates a contract with said stream object to restore its format data at the end of its lifetime. **The format data include: flags, width, precision, and fill.**

The `basic_iosfrstor` type models a unique contract system (similar to `std::unique_ptr`). Copying is prohibited, but move semantics may be used to transfer ownership of the contract. An ADL swap function is present for exchanging contracts. The default constructor creates an instance with no contract.

Methods are included to `release()` the contract (without completing it), to `rstor()` the stream formatting (without releasing the contract), and to check if it's `empty()` (no contract) (may also use `explicit operator bool()`).

With the exception of the constructor, all methods are noexcept. If the constructor throws, no contract is formed.

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
