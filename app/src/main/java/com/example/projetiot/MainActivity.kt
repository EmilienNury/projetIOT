package com.example.projetiot

import android.annotation.SuppressLint
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.projetiot.adapter.AdapterFirebase
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase

private lateinit var database: DatabaseReference

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        var entry: HashMap<String,String>
        var alarm: HashMap<String,String>

        database = Firebase.database.reference
        database.child("Alarm").get().addOnSuccessListener {
            var alarm: HashMap<String,String> = it.value as HashMap<String, String>;
            database.child("Entry").get().addOnSuccessListener {
                var entry: HashMap<String,String> = it.value as HashMap<String, String>;
                val recyclerView = this.findViewById<RecyclerView>(R.id.firebaseRecylerView)
                val adapter = AdapterFirebase(alarm,this)
                val layoutManager : RecyclerView.LayoutManager = LinearLayoutManager(this)
                recyclerView.layoutManager = layoutManager
                recyclerView.adapter = adapter
            }.addOnFailureListener {
                Log.e("firebase", "Error getting data", it)
            }
        }.addOnFailureListener{
            Log.e("firebase", "Error getting data", it)
        }

    }
}