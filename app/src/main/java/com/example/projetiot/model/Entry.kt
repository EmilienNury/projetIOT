package com.example.projetiot.model

import com.google.gson.annotations.SerializedName

data class Entry(
    @SerializedName("offset") val offset: String
)