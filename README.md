# ios-frstor
ios-frstor (i/o stream format restore) is a single C++ header that adds a RAII type called `iosfrstor` for restoring `std::ostream` formats based on a unique contract.

An instance of `iosfrstor` can be explicitly constructed from a `std::ostream`, at which point it creates a contract with said stream object to restore its format data at the end of its lifetime. The format data include: flags, width, precision, and fill.

The `iosfrstor` type models a unique contract system (similar to `std::unique_ptr`). Copying is prohibited, but move semantics may be used to transfer ownership of the contract. An ADL swap function is present for exchanging contracts. The default constructor creates an instance with no contract.

Methods are included to `release()` the contract (without completing it), to `rstor()` the stream formatting (without releasing the contract), and to check if it's `empty()` (no contract) (may also use `explicit operator bool()`).
