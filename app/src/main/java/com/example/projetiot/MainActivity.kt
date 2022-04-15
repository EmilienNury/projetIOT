package com.example.projetiot

import android.annotation.SuppressLint
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import com.example.projetiot.adapter.AdapterFirebase
import com.example.projetiot.model.Data
import com.google.firebase.database.DatabaseReference
import com.google.firebase.database.ktx.database
import com.google.firebase.ktx.Firebase

private lateinit var database: DatabaseReference

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        database = Firebase.database.reference
        database.child("Alarm").get().addOnSuccessListener {
            var alarm: HashMap<String,String> = it.value as HashMap<String, String>;
            database.child("Entry").get().addOnSuccessListener {
                var entry: HashMap<String,String> = it.value as HashMap<String, String>;
                val recyclerView = this.findViewById<RecyclerView>(R.id.firebaseRecylerView)
                var result = HashMap<String, Data>()
                for (a in alarm) {
                    result.put(a.key, Data(a.value, "alarm"))

                }
                for (e in entry) {
                    result.put(e.key, Data(e.value, "entry"))
                }
                val adapter = AdapterFirebase(result as HashMap<String, Data>, this)
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