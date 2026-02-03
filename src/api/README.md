# Fortran API
The Fortran API is generated from the simple YAML markup. For each extern "C" function you need
to add a corresponding header with YAML description. For example:
```C++
/*
@api begin
sirius_set_lattice_vectors:
  doc: Set vectors of the unit cell.
  arguments:
    handler:
      type: void*
      attr: in, required
      doc: Simulation context handler
    a1:
      type: double
      attr: in, required, dimension(3)
      doc: 1st vector
    a2:
      type: double
      attr: in, required, dimension(3)
      doc: 2nd vector
    a3:
      type: double
      attr: in, required, dimension(3)
      doc: 3rd vector
@api end
*/
void sirius_set_lattice_vectors(void*  const* handler__,
                                double const* a1__,
                                double const* a2__,
                                double const* a3__)
{
    auto& sim_ctx = get_sim_ctx(handler__);
    sim_ctx.unit_cell().set_lattice_vectors(vector3d<double>(a1__), vector3d<double>(a2__), vector3d<double>(a3__));
}

```

# Common considerations

By default, all variables are passed by pointer. There are two exceptions to this rule: 
  - all input, required, simple data types (bool, int, double) are passed by value.
  - function pointers are passed by value

Inside C-api all functions are wrapped in `call_sirius` function:

```
call_sirius([&]()
{
  // actual call to sirius 
}, error_code__);
```

## API call logging

All C/Fortran API entry points that use `call_sirius` now emit a simple trace line to stdout on entry:

```
YYYY-MM-DD HH:MM:SS SIRIUS API call: <function_name>
```

This is implemented inside `call_sirius` in [src/api/sirius_api.cpp](src/api/sirius_api.cpp) using the C++ `__func__`
macro, so the logged name matches the public API function being called (for example, `sirius_initialize`).

### Details

- **Where**: Centralized in `call_sirius` so every entry point that wraps work in `call_sirius(...)` is traced.
- **Format**: Local time in `$YYYY-MM-DD HH:MM:SS$` followed by the literal `SIRIUS API call:` and the C API
  function name.
- **Source**: Uses `std::chrono::system_clock` and `std::put_time` for formatting.

Example output:

```
2026-02-03 14:27:05 SIRIUS API call: sirius_initialize
```

On the Fortran side all pointers are declared as
```
type(C_PTR) :: var
```
