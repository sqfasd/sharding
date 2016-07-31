{
  "targets": [
    {
      "target_name": "sharding_",
      "sources": [
        "hash.cc",
        "sharding.cc"
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ]
    }
  ]
}
