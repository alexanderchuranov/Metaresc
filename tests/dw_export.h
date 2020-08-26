{
  .ra = (mr_td_t[]){
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_rarray_t",
        .hash_value = 0
      },
      .size = 32,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "data",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 8,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 24,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "caddr_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_ptrdes_type_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_PD_SAVE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_PD_LOAD",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_substr_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "str",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "length",
                .hash_value = 0
              },
              .type = "size_t",
              .offset = 8,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_quoted_substr_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "substr",
                .hash_value = 0
              },
              .type = "mr_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "unquote",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "string_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_value_type_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 64,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_VOID",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_QUOTED_SUBSTR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_INT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_FLOAT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_COMPLEX",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_CHAR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_VT_ID",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_value_t",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "value_type",
                .hash_value = 0
              },
              .type = "mr_value_type_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type_specific",
                .hash_value = 0
              },
              .type = "char",
              .offset = 4,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = true,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = "mr_type_anonymous_0_t",
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_load_params_t",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 16,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "mr_value",
                .hash_value = 0
              },
              .type = "mr_value_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_save_params_t",
        .hash_value = 0
      },
      .size = 96,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "union_discriminator",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "next_typed",
                .hash_value = 0
              },
              .type = "int",
              .offset = 80,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "next_untyped",
                .hash_value = 0
              },
              .type = "int",
              .offset = 84,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "level",
                .hash_value = 0
              },
              .type = "int",
              .offset = 88,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ptrdes_flags_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "is_null",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 0,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "is_referenced",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 0,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "is_content_reference",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 0,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 2
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "is_opaque_data",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 0,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 3
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ptrdes_t",
        .hash_value = 0
      },
      .size = 272,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 128,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "data",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 8,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "fd",
                .hash_value = 0
              },
              .type = "mr_fd_t",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "idx",
                .hash_value = 0
              },
              .type = "int32_t",
              .offset = 112,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "ref_idx",
                .hash_value = 0
              },
              .type = "int32_t",
              .offset = 116,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "int",
              .offset = 120,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "first_child",
                .hash_value = 0
              },
              .type = "int",
              .offset = 124,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "last_child",
                .hash_value = 0
              },
              .type = "int",
              .offset = 128,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "int",
              .offset = 132,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "int",
              .offset = 136,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "flags",
                .hash_value = 0
              },
              .type = "mr_ptrdes_flags_t",
              .offset = 140,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type_specific",
                .hash_value = 0
              },
              .type = "char",
              .offset = 144,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = true,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = "mr_type_anonymous_1_t",
              .offset = 144,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "res",
                .hash_value = 0
              },
              .type = "mr_res_t",
              .offset = 240,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ra_ptrdes_t",
        .hash_value = 0
      },
      .size = 56,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 48,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ra",
                .hash_value = 0
              },
              .type = "mr_ptrdes_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 8,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "ptrdes_type",
                .hash_value = 0
              },
              .type = "mr_ptrdes_type_t",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "res",
                .hash_value = 0
              },
              .type = "mr_res_t",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "XDR",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 56,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "x_op",
                .hash_value = 0
              },
              .type = "xdr_op",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "x_ops",
                .hash_value = 0
              },
              .type = "xdr_ops",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "x_public",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "x_base",
                .hash_value = 0
              },
              .type = "char",
              .offset = 32,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "x_handy",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 40,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "xdr_handler_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_func_param_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "args",
                .hash_value = 0
              },
              .type = "mr_fd_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_bitfield_param_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "bitfield",
                .hash_value = 0
              },
              .type = "uint8_t",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT8,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "width",
                .hash_value = 0
              },
              .type = "uint8_t",
              .offset = 12,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT8,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "shift",
                .hash_value = 0
              },
              .type = "uint8_t",
              .offset = 13,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_array_param_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "count",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 0,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "row_count",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 4,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_fd_param_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 48,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT8,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "dummy",
                .hash_value = 0
              },
              .type = "uint8_t",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "array_param",
                .hash_value = 0
              },
              .type = "mr_array_param_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "enum_value",
                .hash_value = 0
              },
              .type = "uint64_t",
              .offset = 0,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "bitfield_param",
                .hash_value = 0
              },
              .type = "mr_bitfield_param_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "func_param",
                .hash_value = 0
              },
              .type = "mr_func_param_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_fd_t",
        .hash_value = 0
      },
      .size = 88,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 104,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "mr_type",
                .hash_value = 0
              },
              .type = "mr_type_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "mr_type_aux",
                .hash_value = 0
              },
              .type = "mr_type_t",
              .offset = 4,
              .size = 0,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "unnamed",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 7,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "mr_hashed_string_t",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 24,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "offset",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 32,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 36,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "param",
                .hash_value = 0
              },
              .type = "mr_fd_param_t",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "meta",
                .hash_value = 0
              },
              .type = "char",
              .offset = 56,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "res",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "res_type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 72,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 80,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_fd_ptr_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 16,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "fdp",
                .hash_value = 0
              },
              .type = "mr_fd_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_struct_param_t",
        .hash_value = 0
      },
      .size = 80,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 16,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "field_by_offset",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_rbtree_node_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "left",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 0,
              .size = 4,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "unused",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 3,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "right",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 4,
              .size = 4,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "red",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 7,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_child_idx_t",
        .hash_value = 0
      },
      .size = 1,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LEFT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_RIGHT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_avltree_node_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "left",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 0,
              .size = 4,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "balanced",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 3,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "right",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 4,
              .size = 4,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "longer",
                .hash_value = 0
              },
              .type = "mr_child_idx_t",
              .offset = 7,
              .size = 0,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_tree_node_idx_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "idx",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 0,
              .size = 4,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "bit",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 3,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_tree_node_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "key",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = true,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = "mr_type_anonymous_2_t",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_tree_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "pool",
                .hash_value = 0
              },
              .type = "mr_tree_node_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 8,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ic_static_array_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 16,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "static_array",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 0,
              .size = 0,
              .param = {
                .array_param = {
                  .count = 3,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_hash_fn_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ic_hash_next_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 48,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "hash_fn",
                .hash_value = 0
              },
              .type = "mr_hash_fn_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "hash_table",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 16,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "resize_count",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 20,
              .size = 4,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 31,
                  .shift = 0
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BITFIELD,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "zero_key",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 23,
              .size = 1,
              .param = {
                .bitfield_param = {
                  .bitfield = NULL,
                  .size = 0,
                  .width = 1,
                  .shift = 7
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ic_rarray_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "ra",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 8,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_visit_fn_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_status_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_SUCCESS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_FAILURE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ic_virt_func_t",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 56,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "add",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "del",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "find",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "foreach",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "index",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "free",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_compar_fn_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_ptr_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 56,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "ptr",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "mr_opaque_data",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "string",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "offset",
                .hash_value = 0
              },
              .type = "mr_offset_t",
              .offset = 0,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "uintptr_t",
                .hash_value = 0
              },
              .type = "uintptr_t",
              .offset = 0,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "intptr_t",
                .hash_value = 0
              },
              .type = "intptr_t",
              .offset = 0,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_res_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "data",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 8,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_ic_type_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 64,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_UNINITIALIZED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_UNSORTED_ARRAY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_SORTED_ARRAY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_HASH_NEXT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_STATIC_ARRAY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_RBTREE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_IC_AVLTREE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ic_t",
        .hash_value = 0
      },
      .size = 80,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 72,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "ic_type",
                .hash_value = 0
              },
              .type = "mr_ic_type_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "items_count",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 4,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "context",
                .hash_value = 0
              },
              .type = "mr_res_t",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "key_type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 32,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "compar_fn",
                .hash_value = 0
              },
              .type = "mr_compar_fn_t",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "virt_func",
                .hash_value = 0
              },
              .type = "mr_ic_virt_func_t",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "type_specific",
                .hash_value = 0
              },
              .type = "char",
              .offset = 56,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 0,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = true,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = "mr_type_anonymous_3_t",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_enum_param_t",
        .hash_value = 0
      },
      .size = 96,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "enum_by_value",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "size_effective",
                .hash_value = 0
              },
              .type = "mr_size_t",
              .offset = 80,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "mr_type_effective",
                .hash_value = 0
              },
              .type = "mr_type_t",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BOOL,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "is_bitmask",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 92,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_td_param_t",
        .hash_value = 0
      },
      .size = 96,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT8,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "void_param",
                .hash_value = 0
              },
              .type = "uint8_t",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "enum_param",
                .hash_value = 0
              },
              .type = "mr_enum_param_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "struct_param",
                .hash_value = 0
              },
              .type = "mr_struct_param_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_hashed_string_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "str",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "hash_value",
                .hash_value = 0
              },
              .type = "mr_hash_value_t",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_type_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 288,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_NONE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_STRING",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_CHAR_ARRAY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_CHAR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_VOID",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_BOOL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_INT8",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_UINT8",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_INT16",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_UINT16",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_INT32",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_UINT32",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 11
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_INT64",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 12
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_UINT64",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 13
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_FLOAT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 14
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_COMPLEX_FLOAT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 15
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_DOUBLE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 16
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_COMPLEX_DOUBLE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 17
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_LONG_DOUBLE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 18
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_COMPLEX_LONG_DOUBLE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 19
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_STRUCT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 20
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_ENUM",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 21
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_FUNC_TYPE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 22
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_ENUM_VALUE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 23
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_FUNC",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 24
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_BITFIELD",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 25
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_BITMASK",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 26
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_ARRAY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 27
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_POINTER",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 28
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_UNION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 29
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_ANON_UNION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 30
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_NAMED_ANON_UNION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 31
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_END_ANON_UNION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 32
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_TRAILING_RECORD",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 33
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_TYPE_LAST",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 34
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_td_t",
        .hash_value = 0
      },
      .size = 264,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 104,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "mr_type",
                .hash_value = 0
              },
              .type = "mr_type_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "mr_hashed_string_t",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "mr_size_t",
              .offset = 24,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "attr",
                .hash_value = 0
              },
              .type = "char",
              .offset = 32,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "param",
                .hash_value = 0
              },
              .type = "mr_td_param_t",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "field_by_name",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 136,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "fields_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 216,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "fields",
                .hash_value = 0
              },
              .type = "mr_fd_ptr_t",
              .offset = 224,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "meta",
                .hash_value = 0
              },
              .type = "char",
              .offset = 232,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "res",
                .hash_value = 0
              },
              .type = "mr_ptr_t",
              .offset = 240,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "res_type",
                .hash_value = 0
              },
              .type = "char",
              .offset = 248,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 256,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "xdr_op",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XDR_ENCODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XDR_DECODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XDR_FREE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "__rpc_xdr",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 56,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "x_op",
                .hash_value = 0
              },
              .type = "xdr_op",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "x_ops",
                .hash_value = 0
              },
              .type = "xdr_ops",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "x_public",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "x_base",
                .hash_value = 0
              },
              .type = "char",
              .offset = 32,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "x_handy",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 40,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "xdr_ops",
        .hash_value = 0
      },
      .size = 72,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 80,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_getlong",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_putlong",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_getbytes",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_putbytes",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_getpostn",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_setpostn",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_inline",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_destroy",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "x_control",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "xmlAttributeType",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 88,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_CDATA",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ID",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_IDREF",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_IDREFS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ENTITY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ENTITIES",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NMTOKEN",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NMTOKENS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ENUMERATION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NOTATION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "_xmlAttr",
        .hash_value = 0
      },
      .size = 96,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 104,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlElementType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "children",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "last",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlAttr",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "_xmlAttr",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "doc",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ns",
                .hash_value = 0
              },
              .type = "xmlNs",
              .offset = 72,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "atype",
                .hash_value = 0
              },
              .type = "xmlAttributeType",
              .offset = 80,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "psvi",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "xmlNs",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 56,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNs",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlNsType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "href",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "prefix",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 24,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "context",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "xmlNsType",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 176,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_TEXT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_CDATA_SECTION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_REF_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_PI_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_COMMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_TYPE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_FRAG_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 11
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NOTATION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 12
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_HTML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 13
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DTD_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 14
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 15
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 16
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 17
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NAMESPACE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 18
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_START",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 19
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_END",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 20
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCB_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 21
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "_xmlNs",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 56,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNs",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlNsType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "href",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "prefix",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 24,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "context",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "_xmlDtd",
        .hash_value = 0
      },
      .size = 128,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 136,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlElementType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "children",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "last",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "doc",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "notations",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 72,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "elements",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 80,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "attributes",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "entities",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 96,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "ExternalID",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 104,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "SystemID",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 112,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "pentities",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 120,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "_xmlDoc",
        .hash_value = 0
      },
      .size = 176,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 200,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlElementType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "children",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "last",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "doc",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "compression",
                .hash_value = 0
              },
              .type = "int",
              .offset = 72,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "standalone",
                .hash_value = 0
              },
              .type = "int",
              .offset = 76,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "intSubset",
                .hash_value = 0
              },
              .type = "_xmlDtd",
              .offset = 80,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "extSubset",
                .hash_value = 0
              },
              .type = "_xmlDtd",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "oldNs",
                .hash_value = 0
              },
              .type = "_xmlNs",
              .offset = 96,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "version",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 104,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "encoding",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 112,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "ids",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 120,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "refs",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 128,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "URL",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 136,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "charset",
                .hash_value = 0
              },
              .type = "int",
              .offset = 144,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "dict",
                .hash_value = 0
              },
              .type = "_xmlDict",
              .offset = 152,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "psvi",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 160,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "parseFlags",
                .hash_value = 0
              },
              .type = "int",
              .offset = 168,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "properties",
                .hash_value = 0
              },
              .type = "int",
              .offset = 172,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "xmlElementType",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 176,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_TEXT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_CDATA_SECTION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_REF_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_PI_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_COMMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_TYPE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_FRAG_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 11
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NOTATION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 12
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_HTML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 13
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DTD_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 14
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 15
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 16
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 17
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NAMESPACE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 18
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_START",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 19
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_END",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 20
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCB_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 21
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "_xmlNode",
        .hash_value = 0
      },
      .size = 120,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 136,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlElementType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "children",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "last",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "doc",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ns",
                .hash_value = 0
              },
              .type = "xmlNs",
              .offset = 72,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "content",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 80,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "properties",
                .hash_value = 0
              },
              .type = "_xmlAttr",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "nsDef",
                .hash_value = 0
              },
              .type = "xmlNs",
              .offset = 96,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "psvi",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 104,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT16,
              .mr_type_aux = MR_TYPE_UINT16,
              .unnamed = false,
              .name = {
                .str = "line",
                .hash_value = 0
              },
              .type = "unsigned short",
              .offset = 112,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT16,
              .mr_type_aux = MR_TYPE_UINT16,
              .unnamed = false,
              .name = {
                .str = "extra",
                .hash_value = 0
              },
              .type = "unsigned short",
              .offset = 114,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "xmlNode",
        .hash_value = 0
      },
      .size = 120,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 136,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlElementType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "children",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "last",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "doc",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ns",
                .hash_value = 0
              },
              .type = "xmlNs",
              .offset = 72,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "content",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 80,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "properties",
                .hash_value = 0
              },
              .type = "_xmlAttr",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "nsDef",
                .hash_value = 0
              },
              .type = "xmlNs",
              .offset = 96,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "psvi",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 104,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT16,
              .mr_type_aux = MR_TYPE_UINT16,
              .unnamed = false,
              .name = {
                .str = "line",
                .hash_value = 0
              },
              .type = "unsigned short",
              .offset = 112,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT16,
              .mr_type_aux = MR_TYPE_UINT16,
              .unnamed = false,
              .name = {
                .str = "extra",
                .hash_value = 0
              },
              .type = "unsigned short",
              .offset = 114,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "xmlNodePtr",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_type_anonymous_5_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 88,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_CDATA",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ID",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_IDREF",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_IDREFS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ENTITY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ENTITIES",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NMTOKEN",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NMTOKENS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_ENUMERATION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NOTATION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_type_anonymous_4_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 176,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_TEXT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_CDATA_SECTION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_REF_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_PI_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_COMMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_TYPE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_FRAG_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 11
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NOTATION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 12
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_HTML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 13
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DTD_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 14
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 15
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 16
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 17
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NAMESPACE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 18
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_START",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 19
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_END",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 20
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCB_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 21
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "yyscan_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_token_lloc_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "start",
                .hash_value = 0
              },
              .type = "mr_lloc_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "end",
                .hash_value = 0
              },
              .type = "mr_lloc_t",
              .offset = 12,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "MR_SCM_STYPE",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "value",
                .hash_value = 0
              },
              .type = "mr_value_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "string",
                .hash_value = 0
              },
              .type = "mr_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "id_ivalue",
                .hash_value = 0
              },
              .type = "mr_type_anonymous_6_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "yy_buffer_state",
        .hash_value = 0
      },
      .size = 72,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 104,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "yy_input_file",
                .hash_value = 0
              },
              .type = "FILE",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "yy_ch_buf",
                .hash_value = 0
              },
              .type = "char",
              .offset = 8,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "yy_buf_pos",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "yy_buf_size",
                .hash_value = 0
              },
              .type = "yy_size_t",
              .offset = 24,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "yy_n_chars",
                .hash_value = 0
              },
              .type = "yy_size_t",
              .offset = 32,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_is_our_buffer",
                .hash_value = 0
              },
              .type = "int",
              .offset = 40,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_is_interactive",
                .hash_value = 0
              },
              .type = "int",
              .offset = 44,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_at_bol",
                .hash_value = 0
              },
              .type = "int",
              .offset = 48,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_bs_lineno",
                .hash_value = 0
              },
              .type = "int",
              .offset = 52,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_bs_column",
                .hash_value = 0
              },
              .type = "int",
              .offset = 56,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_fill_buffer",
                .hash_value = 0
              },
              .type = "int",
              .offset = 60,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_buffer_status",
                .hash_value = 0
              },
              .type = "int",
              .offset = 64,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "YY_BUFFER_STATE",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "__sbuf",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_base",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_size",
                .hash_value = 0
              },
              .type = "int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "__sFILE",
        .hash_value = 0
      },
      .size = 152,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 168,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_p",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_r",
                .hash_value = 0
              },
              .type = "int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_w",
                .hash_value = 0
              },
              .type = "int",
              .offset = 12,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT16,
              .mr_type_aux = MR_TYPE_INT16,
              .unnamed = false,
              .name = {
                .str = "_flags",
                .hash_value = 0
              },
              .type = "short",
              .offset = 16,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT16,
              .mr_type_aux = MR_TYPE_INT16,
              .unnamed = false,
              .name = {
                .str = "_file",
                .hash_value = 0
              },
              .type = "short",
              .offset = 18,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_bf",
                .hash_value = 0
              },
              .type = "__sbuf",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_lbfsize",
                .hash_value = 0
              },
              .type = "int",
              .offset = 40,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_cookie",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_close",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_read",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_seek",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 72,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_write",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 80,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_ub",
                .hash_value = 0
              },
              .type = "__sbuf",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_extra",
                .hash_value = 0
              },
              .type = "__sFILEX",
              .offset = 104,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_ur",
                .hash_value = 0
              },
              .type = "int",
              .offset = 112,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_ubuf",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 116,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 3,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_nbuf",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 119,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_lb",
                .hash_value = 0
              },
              .type = "__sbuf",
              .offset = 120,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_blksize",
                .hash_value = 0
              },
              .type = "int",
              .offset = 136,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "_offset",
                .hash_value = 0
              },
              .type = "fpos_t",
              .offset = 144,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "FILE",
        .hash_value = 0
      },
      .size = 152,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 168,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_p",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_r",
                .hash_value = 0
              },
              .type = "int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_w",
                .hash_value = 0
              },
              .type = "int",
              .offset = 12,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT16,
              .mr_type_aux = MR_TYPE_INT16,
              .unnamed = false,
              .name = {
                .str = "_flags",
                .hash_value = 0
              },
              .type = "short",
              .offset = 16,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT16,
              .mr_type_aux = MR_TYPE_INT16,
              .unnamed = false,
              .name = {
                .str = "_file",
                .hash_value = 0
              },
              .type = "short",
              .offset = 18,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_bf",
                .hash_value = 0
              },
              .type = "__sbuf",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_lbfsize",
                .hash_value = 0
              },
              .type = "int",
              .offset = 40,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_cookie",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_close",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_read",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_seek",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 72,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_write",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 80,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_ub",
                .hash_value = 0
              },
              .type = "__sbuf",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_extra",
                .hash_value = 0
              },
              .type = "__sFILEX",
              .offset = 104,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_ur",
                .hash_value = 0
              },
              .type = "int",
              .offset = 112,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_ubuf",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 116,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 3,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "_nbuf",
                .hash_value = 0
              },
              .type = "unsigned char",
              .offset = 119,
              .size = 1,
              .param = {
                .array_param = {
                  .count = 1,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "_lb",
                .hash_value = 0
              },
              .type = "__sbuf",
              .offset = 120,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "_blksize",
                .hash_value = 0
              },
              .type = "int",
              .offset = 136,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "_offset",
                .hash_value = 0
              },
              .type = "fpos_t",
              .offset = 144,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_lloc_t",
        .hash_value = 0
      },
      .size = 12,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "lineno",
                .hash_value = 0
              },
              .type = "int",
              .offset = 0,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "column",
                .hash_value = 0
              },
              .type = "int",
              .offset = 4,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "offset",
                .hash_value = 0
              },
              .type = "int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_load_t",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 48,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "lloc",
                .hash_value = 0
              },
              .type = "mr_lloc_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "str",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "buf",
                .hash_value = 0
              },
              .type = "char",
              .offset = 24,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "int",
              .offset = 32,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ptrs",
                .hash_value = 0
              },
              .type = "mr_ra_ptrdes_t",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "yyguts_t",
        .hash_value = 0
      },
      .size = 168,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 208,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "yyextra_r",
                .hash_value = 0
              },
              .type = "mr_load_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "yyin_r",
                .hash_value = 0
              },
              .type = "FILE",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "yyout_r",
                .hash_value = 0
              },
              .type = "FILE",
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "yy_buffer_stack_top",
                .hash_value = 0
              },
              .type = "size_t",
              .offset = 24,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "yy_buffer_stack_max",
                .hash_value = 0
              },
              .type = "size_t",
              .offset = 32,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "yy_buffer_stack",
                .hash_value = 0
              },
              .type = "YY_BUFFER_STATE",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_CHAR,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "yy_hold_char",
                .hash_value = 0
              },
              .type = "char",
              .offset = 48,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "yy_n_chars",
                .hash_value = 0
              },
              .type = "yy_size_t",
              .offset = 56,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "yyleng_r",
                .hash_value = 0
              },
              .type = "yy_size_t",
              .offset = 64,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "yy_c_buf_p",
                .hash_value = 0
              },
              .type = "char",
              .offset = 72,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_init",
                .hash_value = 0
              },
              .type = "int",
              .offset = 80,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_start",
                .hash_value = 0
              },
              .type = "int",
              .offset = 84,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_did_buffer_switch_on_eof",
                .hash_value = 0
              },
              .type = "int",
              .offset = 88,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_start_stack_ptr",
                .hash_value = 0
              },
              .type = "int",
              .offset = 92,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_start_stack_depth",
                .hash_value = 0
              },
              .type = "int",
              .offset = 96,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_start_stack",
                .hash_value = 0
              },
              .type = "int",
              .offset = 104,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_last_accepting_state",
                .hash_value = 0
              },
              .type = "yy_state_type",
              .offset = 112,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "yy_last_accepting_cpos",
                .hash_value = 0
              },
              .type = "char",
              .offset = 120,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yylineno_r",
                .hash_value = 0
              },
              .type = "int",
              .offset = 128,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_flex_debug_r",
                .hash_value = 0
              },
              .type = "int",
              .offset = 132,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "yytext_r",
                .hash_value = 0
              },
              .type = "char",
              .offset = 136,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_more_flag",
                .hash_value = 0
              },
              .type = "int",
              .offset = 144,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "yy_more_len",
                .hash_value = 0
              },
              .type = "int",
              .offset = 148,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "yylval_r",
                .hash_value = 0
              },
              .type = "MR_SCM_STYPE",
              .offset = 152,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "yylloc_r",
                .hash_value = 0
              },
              .type = "mr_token_lloc_t",
              .offset = 160,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "yyalloc",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT16,
              .mr_type_aux = MR_TYPE_INT16,
              .unnamed = false,
              .name = {
                .str = "yyss_alloc",
                .hash_value = 0
              },
              .type = "yytype_int16",
              .offset = 0,
              .size = 2,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = false,
              .name = {
                .str = "yyvs_alloc",
                .hash_value = 0
              },
              .type = "MR_SCM_STYPE",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "yyls_alloc",
                .hash_value = 0
              },
              .type = "mr_token_lloc_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "MR_CINIT_STYPE",
        .hash_value = 0
      },
      .size = 48,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "value",
                .hash_value = 0
              },
              .type = "mr_value_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "string",
                .hash_value = 0
              },
              .type = "mr_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "id_ivalue",
                .hash_value = 0
              },
              .type = "mr_type_anonymous_7_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "MR_XML1_STYPE",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 16,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "string",
                .hash_value = 0
              },
              .type = "mr_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "node_validator_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_tree_path_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "child_idx",
                .hash_value = 0
              },
              .type = "mr_child_idx_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "idx",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 4,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_tree_traverse_t",
        .hash_value = 0
      },
      .size = 512,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 0,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BOOL,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "equal",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 4,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "path",
                .hash_value = 0
              },
              .type = "mr_tree_path_t",
              .offset = 8,
              .size = 0,
              .param = {
                .array_param = {
                  .count = 63,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_ra_printf_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "xmlDoc",
        .hash_value = 0
      },
      .size = 176,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 200,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "_private",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "xmlElementType",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "children",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "last",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 48,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "prev",
                .hash_value = 0
              },
              .type = "_xmlNode",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "doc",
                .hash_value = 0
              },
              .type = "_xmlDoc",
              .offset = 64,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "compression",
                .hash_value = 0
              },
              .type = "int",
              .offset = 72,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "standalone",
                .hash_value = 0
              },
              .type = "int",
              .offset = 76,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "intSubset",
                .hash_value = 0
              },
              .type = "_xmlDtd",
              .offset = 80,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "extSubset",
                .hash_value = 0
              },
              .type = "_xmlDtd",
              .offset = 88,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "oldNs",
                .hash_value = 0
              },
              .type = "_xmlNs",
              .offset = 96,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "version",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 104,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "encoding",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 112,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "ids",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 120,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "refs",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 128,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "URL",
                .hash_value = 0
              },
              .type = "xmlChar",
              .offset = 136,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "charset",
                .hash_value = 0
              },
              .type = "int",
              .offset = 144,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "dict",
                .hash_value = 0
              },
              .type = "_xmlDict",
              .offset = 152,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "psvi",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 160,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "parseFlags",
                .hash_value = 0
              },
              .type = "int",
              .offset = 168,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "properties",
                .hash_value = 0
              },
              .type = "int",
              .offset = 172,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "xmlDocPtr",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_type_anonymous_8_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 176,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_TEXT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_CDATA_SECTION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_REF_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_PI_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_COMMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_TYPE_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCUMENT_FRAG_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 11
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NOTATION_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 12
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_HTML_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 13
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DTD_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 14
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ELEMENT_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 15
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ATTRIBUTE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 16
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_ENTITY_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 17
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_NAMESPACE_DECL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 18
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_START",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 19
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_XINCLUDE_END",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 20
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "XML_DOCB_DOCUMENT_NODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 21
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_xml_esc_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "esc_seq",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "length",
                .hash_value = 0
              },
              .type = "int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_CHAR,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "symbol",
                .hash_value = 0
              },
              .type = "char",
              .offset = 12,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_load_node_context_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "idx",
                .hash_value = 0
              },
              .type = "int",
              .offset = 0,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ptrs",
                .hash_value = 0
              },
              .type = "mr_ra_ptrdes_t",
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_process_quoted_str_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_load_data_t",
        .hash_value = 0
      },
      .size = 80,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ptrs",
                .hash_value = 0
              },
              .type = "mr_ra_ptrdes_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_ra_idx_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 56,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "mr_ra_idx",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 64,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_ra_idx_alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 72,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_load_handler_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_check_ud_ctx_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "mr_save_data",
                .hash_value = 0
              },
              .type = "mr_save_data_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "node",
                .hash_value = 0
              },
              .type = "int",
              .offset = 8,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "parent",
                .hash_value = 0
              },
              .type = "int",
              .offset = 12,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_ptrdes_processor_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_union_discriminator_t",
        .hash_value = 0
      },
      .size = 40,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "type",
                .hash_value = 0
              },
              .type = "mr_hashed_string_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "discriminator",
                .hash_value = 0
              },
              .type = "mr_hashed_string_t",
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "fdp",
                .hash_value = 0
              },
              .type = "mr_fd_t",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_save_data_t",
        .hash_value = 0
      },
      .size = 264,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 80,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ptrs",
                .hash_value = 0
              },
              .type = "mr_ra_ptrdes_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "typed_ptrs",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "untyped_ptrs",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 136,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_ra_idx_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 216,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "mr_ra_idx",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 224,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_ra_idx_alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 232,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_ra_ud_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 240,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "mr_ra_ud",
                .hash_value = 0
              },
              .type = "mr_union_discriminator_t",
              .offset = 248,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "mr_ra_ud_alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 256,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_save_handler_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "dl_info",
        .hash_value = 0
      },
      .size = 32,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "dli_fname",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "dli_fbase",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "dli_sname",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "dli_saddr",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "Dl_info",
        .hash_value = 0
      },
      .size = 32,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "dli_fname",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "dli_fbase",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "dli_sname",
                .hash_value = 0
              },
              .type = "char",
              .offset = 16,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "dli_saddr",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_func_name_t",
        .hash_value = 0
      },
      .size = 16,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UNION,
              .mr_type_aux = MR_TYPE_UNION,
              .unnamed = true,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = "mr_type_anonymous_9_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "name",
                .hash_value = 0
              },
              .type = "char",
              .offset = 8,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_ra_fn_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 32,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "ra",
                .hash_value = 0
              },
              .type = "mr_func_name_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 8,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "alloc_size",
                .hash_value = 0
              },
              .type = "ssize_t",
              .offset = 16,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ARRAY,
      .type = {
        .str = "__builtin_va_list",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ARRAY,
      .type = {
        .str = "va_list",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "__va_list_tag",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "gp_offset",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 0,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT32,
              .mr_type_aux = MR_TYPE_UINT32,
              .unnamed = false,
              .name = {
                .str = "fp_offset",
                .hash_value = 0
              },
              .type = "unsigned int",
              .offset = 4,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "overflow_arg_area",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "reg_save_area",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_message_id_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 480,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_OUT_OF_MEMORY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_END",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_DATA",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNBALANCED_TAGS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_XDR_WRONG_ENCODING_MODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_XDR_SAVE_FAILED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_XML_SAVE_FAILED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_LOAD_STRUCT_FAILED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_LOAD_METHOD_MISSED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 8
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_XDR_LOAD_FAILED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 9
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_NO_TYPE_DESCRIPTOR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 10
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_TYPE_NOT_STRUCT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 11
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_TYPE_NOT_UNION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 12
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_TYPE_NOT_ENUM",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 13
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_NULL_POINTER",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 14
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_IDS_COLLISION",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 15
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNDEFINED_REF_IDX",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 16
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNKNOWN_ENUM",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 17
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_READ_INT",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 18
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_READ_LONG_DOUBLE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 19
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_READ_COMPLEX_LONG_DOUBLE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 20
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_READ_CHAR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 21
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_READ_FUNC",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 22
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_STRING_TRUNCATED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 23
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNKNOWN_SUBNODE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 24
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_SAVE_IDX_RANGE_CHECK",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 25
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_NODE_NAME_MISSMATCH",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 26
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_NODE_TYPE_MISSMATCH",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 27
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNSUPPORTED_NODE_TYPE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 28
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_RANGE_CHECK",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 29
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_READ_REF",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 30
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_SAVE_ENUM",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 31
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_NOT_BITMASK",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 32
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_SAVE_BITMASK",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 33
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_DUPLICATED_ENUMS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 34
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_INT_OF_UNKNOWN_SIZE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 35
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_PARSE_ERROR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 36
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_ANON_UNION_TYPE_ERROR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 37
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNKNOWN_XML_ESC",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 38
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_WRONG_XML_ESC",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 39
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNION_DISCRIMINATOR_ERROR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 40
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_XML_STRING_ENCODING_FAILED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 41
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_POINTER_NODE_CHILD_MISSING",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 42
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_NULL_POINTER",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 43
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_STRING_SAVE_DATA",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 44
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_WRONG_RESULT_TYPE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 45
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_DIVISION_BY_ZERO",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 46
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_TARGET_TYPE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 47
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_DATA_AT_THE_END",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 48
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_SERIALIZATION_FAILED",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 49
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_IC_TYPE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 50
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_HASH_TABLE_ERROR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 51
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_CHARS_AFTER_CLOSING_TAG",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 52
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_TAGS_DONT_MATCH",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 53
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_CANT_READ_PROPERTY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 54
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_WRONG_SIZE_FOR_DYNAMIC_ARRAY",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 55
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_MR_TYPE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 56
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_UNEXPECTED_NUMBER_OF_ITEMS",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 57
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_MESSAGE_LAST",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 58
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_FUNC_TYPE,
      .type = {
        .str = "mr_msg_handler_t",
        .hash_value = 0
      },
      .size = 0,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 0,
      .fields = NULL,
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_ENUM,
      .type = {
        .str = "mr_log_level_t",
        .hash_value = 0
      },
      .size = 4,
      .attr = NULL,
      .param = {
        .enum_param = {
          .enum_by_value = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          },
          .size_effective = 0,
          .mr_type_effective = MR_TYPE_NONE,
          .is_bitmask = false
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 72,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_ALL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 0
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_TRACE",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 1
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_DEBUG",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 2
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_INFO",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 3
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_WARN",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 4
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_ERROR",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 5
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_FATAL",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 6
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM_VALUE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "MR_LL_OFF",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .param = {
                .enum_value = 7
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_mem_t",
        .hash_value = 0
      },
      .size = 32,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FLOAT,
              .mr_type_aux = MR_TYPE_FLOAT,
              .unnamed = false,
              .name = {
                .str = "mem_alloc_strategy",
                .hash_value = 0
              },
              .type = "float",
              .offset = 0,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "calloc",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 8,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "realloc",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 16,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "free",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 24,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_conf_t",
        .hash_value = 0
      },
      .size = 568,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 72,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "mr_mem",
                .hash_value = 0
              },
              .type = "mr_mem_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ENUM,
              .mr_type_aux = MR_TYPE_ENUM,
              .unnamed = false,
              .name = {
                .str = "log_level",
                .hash_value = 0
              },
              .type = "mr_log_level_t",
              .offset = 32,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_FUNC_TYPE,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "msg_handler",
                .hash_value = 0
              },
              .type = "mr_msg_handler_t",
              .offset = 40,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_BOOL,
              .mr_type_aux = MR_TYPE_BOOL,
              .unnamed = false,
              .name = {
                .str = "cache_func_resolve",
                .hash_value = 0
              },
              .type = "_Bool",
              .offset = 48,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "enum_by_name",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 56,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "fields_names",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 136,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "type_by_name",
                .hash_value = 0
              },
              .type = "mr_ic_t",
              .offset = 216,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "output_format",
                .hash_value = 0
              },
              .type = "mr_ra_printf_t",
              .offset = 296,
              .size = 0,
              .param = {
                .array_param = {
                  .count = 34,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_type_anonymous_0_t",
        .hash_value = 0
      },
      .size = 32,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 64,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT8,
              .mr_type_aux = MR_TYPE_UINT8,
              .unnamed = false,
              .name = {
                .str = "vt_void",
                .hash_value = 0
              },
              .type = "uint8_t",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT64,
              .mr_type_aux = MR_TYPE_INT64,
              .unnamed = false,
              .name = {
                .str = "vt_int",
                .hash_value = 0
              },
              .type = "long long int",
              .offset = 0,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_LONG_DOUBLE,
              .mr_type_aux = MR_TYPE_LONG_DOUBLE,
              .unnamed = false,
              .name = {
                .str = "vt_float",
                .hash_value = 0
              },
              .type = "long double",
              .offset = 0,
              .size = 16,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_COMPLEX_LONG_DOUBLE,
              .mr_type_aux = MR_TYPE_COMPLEX_LONG_DOUBLE,
              .unnamed = false,
              .name = {
                .str = "vt_complex",
                .hash_value = 0
              },
              .type = "complex",
              .offset = 0,
              .size = 32,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "vt_string",
                .hash_value = 0
              },
              .type = "string_t",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_CHAR,
              .mr_type_aux = MR_TYPE_CHAR,
              .unnamed = false,
              .name = {
                .str = "vt_char",
                .hash_value = 0
              },
              .type = "char",
              .offset = 0,
              .size = 1,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "vt_quoted_substr",
                .hash_value = 0
              },
              .type = "mr_quoted_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_type_anonymous_1_t",
        .hash_value = 0
      },
      .size = 96,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "save_params",
                .hash_value = 0
              },
              .type = "mr_save_params_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "load_params",
                .hash_value = 0
              },
              .type = "mr_load_params_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_type_anonymous_2_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 40,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_ARRAY,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "next",
                .hash_value = 0
              },
              .type = "mr_tree_node_idx_t",
              .offset = 0,
              .size = 0,
              .param = {
                .array_param = {
                  .count = 2,
                  .row_count = 1
                }
              },
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "root",
                .hash_value = 0
              },
              .type = "mr_tree_node_idx_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "avl",
                .hash_value = 0
              },
              .type = "mr_avltree_node_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "rb",
                .hash_value = 0
              },
              .type = "mr_rbtree_node_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_type_anonymous_3_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 64,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "void_ptr",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "rarray",
                .hash_value = 0
              },
              .type = "mr_ic_rarray_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "hash_next",
                .hash_value = 0
              },
              .type = "mr_ic_hash_next_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "static_array",
                .hash_value = 0
              },
              .type = "mr_ic_static_array_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "rb",
                .hash_value = 0
              },
              .type = "mr_tree_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "avl",
                .hash_value = 0
              },
              .type = "mr_tree_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "tree",
                .hash_value = 0
              },
              .type = "mr_tree_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_type_anonymous_6_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "id",
                .hash_value = 0
              },
              .type = "mr_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "ivalue",
                .hash_value = 0
              },
              .type = "int",
              .offset = 16,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_STRUCT,
      .type = {
        .str = "mr_type_anonymous_7_t",
        .hash_value = 0
      },
      .size = 24,
      .attr = NULL,
      .param = {
        .struct_param = {
          .field_by_offset = {
            .ic_type = MR_IC_UNINITIALIZED,
            .items_count = 0,
            .context = {
              .data = {
                NULL
              },
              .type = NULL,
              .mr_size = 0
            },
            .key_type = NULL,
            .compar_fn = NULL,
            .virt_func = NULL,
            .type_specific = "", {
              .void_ptr = NULL
            }
          }
        }
      },
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_STRUCT,
              .mr_type_aux = MR_TYPE_STRUCT,
              .unnamed = false,
              .name = {
                .str = "id",
                .hash_value = 0
              },
              .type = "mr_substr_t",
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_INT32,
              .mr_type_aux = MR_TYPE_INT32,
              .unnamed = false,
              .name = {
                .str = "ivalue",
                .hash_value = 0
              },
              .type = "int",
              .offset = 16,
              .size = 4,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    },
    {
      .mr_type = MR_TYPE_UNION,
      .type = {
        .str = "mr_type_anonymous_9_t",
        .hash_value = 0
      },
      .size = 8,
      .attr = NULL,
      .field_by_name = {
        .ic_type = MR_IC_UNINITIALIZED,
        .items_count = 0,
        .context = {
          .data = {
            NULL
          },
          .type = NULL,
          .mr_size = 0
        },
        .key_type = NULL,
        .compar_fn = NULL,
        .virt_func = NULL,
        .type_specific = "", {
          .void_ptr = NULL
        }
      },
      .fields_size = 24,
      .fields = (mr_fd_ptr_t[]){
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_UINT64,
              .mr_type_aux = MR_TYPE_UINT64,
              .unnamed = false,
              .name = {
                .str = "func_",
                .hash_value = 0
              },
              .type = "uintptr_t",
              .offset = 0,
              .size = 8,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_POINTER,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = "func",
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        },
        {
          .fdp = (mr_fd_t[]){
            {
              .mr_type = MR_TYPE_TRAILING_RECORD,
              .mr_type_aux = MR_TYPE_NONE,
              .unnamed = false,
              .name = {
                .str = NULL,
                .hash_value = 0
              },
              .type = NULL,
              .offset = 0,
              .size = 0,
              .meta = NULL,
              .res = {
                NULL
              },
              .res_type = NULL,
              .mr_size = 0
            }
          }
        }
      },
      .meta = NULL,
      .res = {
        NULL
      },
      .res_type = NULL,
      .mr_size = 0
    }
  },
  .size = 29304
}
