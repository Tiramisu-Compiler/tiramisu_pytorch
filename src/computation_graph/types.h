namespace tiramisu_torch{

enum node_t
{
    node_input,          // 
    node_op,          // 
};

enum primitive_t
{
    p_uint8,
    p_uint16,
    p_uint32,
    p_uint64,
    p_int8,
    p_int16,
    p_int32,
    p_int64,
    p_float32,
    p_float64,
    p_boolean,
    p_async,
    p_wait_ptr,
    p_void_ptr,  // Used for raw buffers in cuda_ast
    p_none
};

}