package com.example.projetiot.adapter

import android.content.Context
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.TextView
import androidx.recyclerview.widget.RecyclerView
import com.example.projetiot.R

class AdapterFirebase(val alarm: HashMap<String,String>, val context: Context ) :
    RecyclerView.Adapter<AdapterFirebase.CustomViewHolder>() {

    class CustomViewHolder(mView: View) : RecyclerView.ViewHolder(mView){
        val date: TextView = mView.findViewById(R.id.hour)
        val type: TextView = mView.findViewById(R.id.type)
    }

    override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): CustomViewHolder {
        val layoutInflater = LayoutInflater.from(parent.context)
        val view: View = layoutInflater.inflate(R.layout.data,parent,false)
        return CustomViewHolder(view)
    }

    override fun onBindViewHolder(holder: CustomViewHolder, position: Int) {
        var values: MutableCollection<String> = alarm.values
        holder.date.text = values.elementAt(position)
        holder.type.text = "Détection"
    }

    override fun getItemCount(): Int {
        return alarm.size
    }
}

