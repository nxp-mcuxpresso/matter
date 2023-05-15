#!/bin/bash

patch_sdk()
{
    echo "MCUXpresso SDK folder \"$1\" has been patched!"
}

main()
{
    if [ $# != 0 ]; then
        echo >&2 "Trailing arguments: $@"
        # 128 for Invalid arguments
        exit 128
    fi

    patch_sdk ../repo
}

main "$@"
